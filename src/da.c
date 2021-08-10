
#include "da.h"
#include <stdlib.h>
#include <assert.h>

static inline unsigned int umax(unsigned int a, unsigned int b)
{
	return a > b ? a : b;
}

void da_resize(da_t* da, unsigned int elem_size, unsigned int new_len)
{
	#ifdef DEBUG
		assert(da->taken_ptr_count == 0);
	#endif
	if (new_len > da->cap)
	{
		unsigned int new_cap =
			umax(new_len, ((float)da->cap + 2.3f) * 1.3f);
		void* new_arr = realloc(da->arr, new_cap * elem_size);
		assert(new_arr != NULL);
		da->arr = new_arr;
		da->cap = new_cap;
	}
	da->len = new_len;
}

unsigned int da_add_len(da_t* da, unsigned int elem_size, int len_inc)
{
	unsigned int old_len = da->len;
	da_resize(da, elem_size, da->len + len_inc);
	return old_len;
}

unsigned int da_add_one(da_t* da, unsigned int elem_size)
{
	return da_add_len(da, elem_size, 1);
}

#ifdef DEBUG

void* da_take_ptr(da_t* da, unsigned int elem_size, unsigned int index)
{
	da->taken_ptr_count++;
	return (char*)da->arr + index * elem_size;
}

void da_giveback_ptr(da_t* da, unsigned int elem_size, void* ptr)
{
	assert((char*)da->arr <= (char*)ptr);
	assert((char*)ptr + elem_size <= (char*)da->arr + da->len * elem_size);
	assert(da->taken_ptr_count >= 1);
	da->taken_ptr_count--;
}

#endif /* DEBUG */
