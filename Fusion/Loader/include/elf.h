#pragma once
#include "common.h"

// Credits to jogolden for the ELF Loading.

// error codes
#define LDR_SUCCESS			0
#define LDR_INVALID_ELF		1
#define LDR_SIZE_ERROR		2
#define LDR_MAP_ERROR		3
#define LDR_RELOC_ERROR		4

int elf_mapped_size(void* elf, size_t* msize);

// functions
// elf - data where elf is stored
// size - size of elf
// exec - memory allocated RWX
// msize - size of the allocated memory
int load_elf(void* elf, size_t size, void* exec, size_t msize, void** entry);