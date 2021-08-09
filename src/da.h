
#ifndef WHYCRYSTALS_HEADER_DA__
#define WHYCRYSTALS_HEADER_DA__

struct da_t
{
	unsigned int len;
	unsigned int cap;
	void* arr;

	#ifdef DEBUG
		unsigned int taken_ptr_count;
	#endif
};
typedef struct da_t da_t;

void da_resize(da_t* da, unsigned int elem_size,
	unsigned int new_len);

unsigned int da_add_len(da_t* da, unsigned int elem_size,
	int len_inc);

unsigned int da_add_one(da_t* da, unsigned int elem_size);

#ifdef DEBUG

void* da_take_ptr(da_t* da, unsigned int elem_size,
	unsigned int index);

void da_return_ptr(da_t* da, unsigned int elem_size, void* ptr);

#else

inline void* da_take_ptr(da_t* da, unsigned int elem_size,
	unsigned int index)
{
	return (char*)da->arr + index * elem_size;
}

inline void da_return_ptr(da_t* da, void* ptr)
{
	(void)da;
	(void)ptr;
}

#endif /* DEBUG */

#endif /* WHYCRYSTALS_HEADER_DA__ */
