
#ifndef WHYCRYSTALS_HEADER_DARRAY__
#define WHYCRYSTALS_HEADER_DARRAY__

struct darray_t
{
	unsigned int len;
	unsigned int cap;
	void* array;

	#ifdef DEBUG
		unsigned int taken_ptr_count;
	#endif
};
typedef struct darray_t darray_t;

void darray_resize(darray_t* darray, unsigned int elem_size,
	unsigned int new_len);

unsigned int darray_add_len(darray_t* darray, unsigned int elem_size,
	int len_inc);

unsigned int darray_add_one(darray_t* darray, unsigned int elem_size);

#ifdef DEBUG

void* darray_take_ptr(darray_t* darray, unsigned int elem_size,
	unsigned int index);

void darray_return_ptr(darray_t* darray, unsigned int elem_size, void* ptr);

#else

inline void* darray_take_ptr(darray_t* darray, unsigned int elem_size,
	unsigned int index)
{
	(void)darray;
	(void)elem_size;
	(void)index;
}

inline void darray_return_ptr(darray_t* darray, void* ptr)
{
	(void)darray;
	(void)ptr;
}

#endif /* DEBUG */

#endif /* WHYCRYSTALS_HEADER_DARRAY__ */
