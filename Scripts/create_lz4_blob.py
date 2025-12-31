#!/usr/bin/env python3
"""
LZ4 block compressor compatible with the minimal C implementation.
Produces raw LZ4 block format (no frame headers).
"""

import struct
import sys
from pathlib import Path

HASH_LOG = 12
HASH_SIZE = 1 << HASH_LOG
MIN_MATCH = 4
MAX_DISTANCE = 0xFFFF
LAST_LITERALS = 5
MF_LIMIT = 12

def hash_pos(val: int) -> int:
    return ((val * 2654435761) & 0xFFFFFFFF) >> (32 - HASH_LOG)

def read32(data: bytes, pos: int) -> int:
    return struct.unpack_from('<I', data, pos)[0]

def write_varlen(length: int) -> bytes:
    result = bytearray()
    while length >= 255:
        result.append(255)
        length -= 255
    result.append(length)
    return bytes(result)

def lz4_compress(src: bytes) -> bytes:
    src_size = len(src)
    if src_size == 0:
        return b''
    
    hash_table = [0] * HASH_SIZE
    dst = bytearray()
    
    src_limit = src_size - LAST_LITERALS
    mf_limit = src_size - MF_LIMIT
    
    anchor = 0
    pos = 0
    
    if src_size >= MIN_MATCH:
        hash_table[hash_pos(read32(src, 0))] = 0
        pos = 1
    
    while pos < mf_limit:
        h = hash_pos(read32(src, pos))
        ref_pos = hash_table[h]
        hash_table[h] = pos
        
        distance = pos - ref_pos
        if distance == 0 or distance > MAX_DISTANCE:
            pos += 1
            continue
        
        if src[ref_pos:ref_pos+4] != src[pos:pos+4]:
            pos += 1
            continue
        
        # found a match
        literal_len = pos - anchor
        
        # extend match
        match_start = pos
        pos += MIN_MATCH
        ref_pos += MIN_MATCH
        while pos < src_limit and src[pos] == src[ref_pos]:
            pos += 1
            ref_pos += 1
        match_len = pos - match_start
        
        # encode token
        lit_token = min(literal_len, 15)
        match_token = min(match_len - MIN_MATCH, 15)
        dst.append((lit_token << 4) | match_token)
        
        # encode literal length overflow
        if literal_len >= 15:
            dst.extend(write_varlen(literal_len - 15))
        
        # copy literals
        dst.extend(src[anchor:anchor + literal_len])
        
        # encode offset (little endian)
        dst.append(distance & 0xFF)
        dst.append((distance >> 8) & 0xFF)
        
        # encode match length overflow
        if match_len - MIN_MATCH >= 15:
            dst.extend(write_varlen(match_len - MIN_MATCH - 15))
        
        anchor = pos
    
    # last literals
    last_literals = src_size - anchor
    lit_token = min(last_literals, 15)
    dst.append(lit_token << 4)
    
    if last_literals >= 15:
        dst.extend(write_varlen(last_literals - 15))
    
    dst.extend(src[anchor:])
    
    return bytes(dst)

# ----------------- Build integration -----------------

MAGIC = int.from_bytes(b'OSM\x00', 'little')

def compress_file(input_path: Path, output_path: Path):
    data = input_path.read_bytes()
    compressed = lz4_compress(data)
    
    header = struct.pack('<III', MAGIC, len(data), len(compressed))
    output_path.write_bytes(header + compressed)
    
    ratio = len(compressed) / len(data) * 100 if data else 0
    print(f"{input_path.name}: {len(data)} -> {len(compressed)} ({ratio:.1f}%)")

def decompress_file(input_path: Path, output_path: Path):
    """For testing - verify round-trip works"""
    from your_test_module import lz4_decompress  # or implement in Python
    data = input_path.read_bytes()
    magic, dec_size, comp_size = struct.unpack_from('<III', data, 0)
    assert magic == MAGIC
    compressed = data[12:12+comp_size]
    # decompress and write...

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input> <output>")
        sys.exit(1)
    
    compress_file(Path(sys.argv[1]), Path(sys.argv[2]))