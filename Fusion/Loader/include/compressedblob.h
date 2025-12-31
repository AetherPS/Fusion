#pragma once

#define BLOB_MAGIC (('O') | ('S' << 8) | ('M' << 16))

typedef struct {
	uint32_t magic;
	uint32_t decompressed_size;
	uint32_t compressed_size;
} BlobHeader;

uint8_t* DecompressBlob(const uint8_t* blobData, size_t* outSize);
int DecompressToFile(const uint8_t* blobData, const char* file);