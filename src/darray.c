
#include "darray.h"
#include <stdlib.h>
#include <assert.h>

#if 0
void darray_reserve(darray_t* darray, unsigned int elem_size,
	unsigned int for_how_much)
{
	if (for_how_much > darray->cap - darray->len)
	{
		/* TODO */
	}
}
#endif

static inline unsigned int umax(unsigned int a, unsigned int b)
{
	return a > b ? a : b;
}

void darray_resize(darray_t* darray, unsigned int elem_size,
	unsigned int new_len)
{
	if (new_len > darray->cap)
	{
		unsigned int new_cap =
			umax(new_len, ((float)darray->cap + 2.3f) * 1.3f);
		void* new_array = realloc(darray->array, new_cap * elem_size);
		assert(new_array != NULL);
		darray->array = new_array;
		darray->cap = new_cap;
	}
	darray->len = new_len;
}

unsigned int darray_add_len(darray_t* darray, unsigned int elem_size,
	int len_inc)
{
	unsigned int old_len = darray->len;
	darray_resize(darray, elem_size, darray->len + len_inc);
	return old_len;
}

unsigned int darray_add_one(darray_t* darray, unsigned int elem_size)
{
	return darray_add_len(darray, elem_size, 1);
}
