#include "common.h"
#include "compressedblob.h"
#include "tinf.h"

uint8_t* DecompressBlob(const uint8_t* blobData, size_t* outSize)
{
	const BlobHeader* hdr = (const BlobHeader*)blobData;

	if (hdr->magic != BLOB_MAGIC)
		return NULL;

	uint8_t* dst = (uint8_t*)malloc(hdr->decompressed_size);
	if (!dst)
		return NULL;

	unsigned int destLen = hdr->decompressed_size;
	int result = tinf_zlib_uncompress(
		dst,
		&destLen,
		blobData + sizeof(BlobHeader),
		hdr->compressed_size
	);

	if (result != TINF_OK || destLen != hdr->decompressed_size)
	{
		free(dst);
		return NULL;
	}

	// Verify CRC32 checksum
	unsigned int actual_crc32 = tinf_crc32(dst, destLen);
	if (actual_crc32 != hdr->crc32)
	{
		free(dst);
		return NULL;
	}

	if (outSize)
		*outSize = destLen;

	return dst;
}

int DecompressToFile(const uint8_t* blobData, const char* file)
{
	size_t decompressedSize;
	uint8_t* decmopressedBlob = DecompressBlob(blobData, &decompressedSize);

	int result = WriteFile(file, decmopressedBlob, decompressedSize);
	free(decmopressedBlob);

	return result;
}