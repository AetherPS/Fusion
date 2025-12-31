#pragma once

#define HASH_LOG       12
#define HASH_SIZE      (1 << HASH_LOG)
#define MIN_MATCH      4
#define MAX_DISTANCE   65535
#define LAST_LITERALS  5
#define MF_LIMIT       12

size_t lz4_compress(const uint8_t* src, size_t src_size, uint8_t* dst, size_t dst_capacity);
size_t lz4_decompress(const uint8_t* src, size_t src_size, uint8_t* dst, size_t dst_capacity);