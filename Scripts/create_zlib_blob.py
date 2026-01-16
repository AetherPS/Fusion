#!/usr/bin/env python3
"""
Zlib compressor compatible with the minimal C implementation.
Produces zlib-wrapped DEFLATE format (RFC 1950).
"""

import struct
import sys
import zlib
from pathlib import Path

# Magic number: 'OSM\x00' in little-endian
MAGIC = int.from_bytes(b'OSM\x00', 'little')

def compress_file(input_path: Path, output_path: Path):
    """Compress a file using zlib and add the blob header."""
    data = input_path.read_bytes()

    # Compress using zlib (DEFLATE with zlib wrapper)
    # Level 9 = maximum compression
    compressed = zlib.compress(data, level=9)

    # Calculate CRC32 of the original data for integrity checking
    crc32_value = zlib.crc32(data) & 0xFFFFFFFF

    # Create blob header: magic (4 bytes) + decompressed_size (4 bytes) + compressed_size (4 bytes) + crc32 (4 bytes)
    header = struct.pack('<IIII', MAGIC, len(data), len(compressed), crc32_value)

    # Write header + compressed data
    output_path.write_bytes(header + compressed)

    # Calculate and print compression ratio
    ratio = len(compressed) / len(data) * 100 if data else 0
    print(f"{input_path.name}: {len(data)} -> {len(compressed)} ({ratio:.1f}%), CRC32: 0x{crc32_value:08X}")

def decompress_file(input_path: Path, output_path: Path):
    """Decompress a blob file for testing."""
    data = input_path.read_bytes()

    # Read header
    magic, dec_size, comp_size, expected_crc32 = struct.unpack_from('<IIII', data, 0)

    if magic != MAGIC:
        raise ValueError(f"Invalid magic: expected 0x{MAGIC:08X}, got 0x{magic:08X}")

    # Extract compressed data (skip 16-byte header)
    compressed = data[16:16+comp_size]

    # Decompress using zlib
    decompressed = zlib.decompress(compressed)

    if len(decompressed) != dec_size:
        raise ValueError(f"Decompressed size mismatch: expected {dec_size}, got {len(decompressed)}")

    # Verify CRC32 checksum
    actual_crc32 = zlib.crc32(decompressed) & 0xFFFFFFFF
    if actual_crc32 != expected_crc32:
        raise ValueError(f"CRC32 mismatch: expected 0x{expected_crc32:08X}, got 0x{actual_crc32:08X}")

    # Write decompressed data
    output_path.write_bytes(decompressed)
    print(f"Decompressed {input_path.name} -> {output_path.name} ({len(decompressed)} bytes, CRC32: 0x{actual_crc32:08X})")

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input> <output>")
        print(f"       {sys.argv[0]} -d <input> <output>  (decompress)")
        sys.exit(1)

    if sys.argv[1] == '-d':
        if len(sys.argv) < 4:
            print(f"Usage: {sys.argv[0]} -d <input> <output>")
            sys.exit(1)
        decompress_file(Path(sys.argv[2]), Path(sys.argv[3]))
    else:
        compress_file(Path(sys.argv[1]), Path(sys.argv[2]))
