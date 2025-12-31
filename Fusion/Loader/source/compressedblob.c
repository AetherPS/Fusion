#include "common.h"
#include "compressedblob.h"
#include "Lz4.h"

uint8_t* DecompressBlob(const uint8_t* blobData, size_t* outSize)
{
	const BlobHeader* hdr = (const BlobHeader*)blobData;

	if (hdr->magic != BLOB_MAGIC)
		return NULL;

	uint8_t* dst = (uint8_t*)malloc(hdr->decompressed_size);
	if (!dst)
		return NULL;

	size_t result = lz4_decompress(
		blobData + sizeof(BlobHeader),
		hdr->compressed_size,
		dst,
		hdr->decompressed_size
	);

	if (result != hdr->decompressed_size)
	{
		free(dst);
		return NULL;
	}

	if (outSize)
		*outSize = result;

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