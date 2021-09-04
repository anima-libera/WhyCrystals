
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
