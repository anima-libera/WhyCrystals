
#include "random.h"
#include <stdlib.h>
#include <string.h>

unsigned int umax(unsigned int a, unsigned int b)
{
	return a > b ? a : b;
}

void* malloc_memcpy(unsigned int size, void* src)
{
	void* buffer = malloc(size);
	memcpy(buffer, src, size);
	return buffer;
}

void* malloc_memcpy_2(
	unsigned int size_1, void* src_1,
	unsigned int size_2, void* src_2)
{
	void* buffer = malloc(size_1 + size_2);
	memcpy(buffer, src_1, size_1);
	memcpy(((char*)buffer) + size_1, src_2, size_2);
	return buffer;
}

/* TODO: Make this better. */
void swap(unsigned int elem_size, void* arr,
	unsigned int index_1, unsigned int index_2)
{
	char* elem_1 = &((char*)arr)[index_1 * elem_size];
	char* elem_2 = &((char*)arr)[index_2 * elem_size];
	char tmp[8];
	memcpy(tmp, elem_1, elem_size);
	memcpy(elem_1, elem_2, elem_size);
	memcpy(elem_2, tmp, elem_size);
}

/* TODO: Make this better. */
void shuffle(unsigned int elem_size, unsigned int len, void* arr, rg_t* rg)
{
	/* Common shuffling algorithm that behaves as uniformly as expected,
	 * given that the random generator behaves uniformly. */
	for (unsigned int i = 0; i < len-1; i++)
	{
		swap(elem_size, arr, i, rg_int(rg, i, len-1));
	}
}
