
#ifndef CTC_HEADER_DARRAY__
#define CTC_HEADER_DARRAY__

struct darray_t
{
	unsigned int len;
	unsigned int cap;
	void* array;
};
typedef struct darray_t darray_t;

void darray_resize(darray_t* darray, unsigned int elem_size,
	unsigned int new_len);

unsigned int darray_add_len(darray_t* darray, unsigned int elem_size,
	int len_inc);

unsigned int darray_add_one(darray_t* darray, unsigned int elem_size);

#endif /* CTC_HEADER_DARRAY__ */
