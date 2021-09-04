
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
