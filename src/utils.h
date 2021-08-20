
#ifndef WHYCRYSTALS_HEADER_UTILS__
#define WHYCRYSTALS_HEADER_UTILS__

#include <stdlib.h>
#include <assert.h>

unsigned int umax(unsigned int a, unsigned int b);

#define DA_LENGTHEN(len_expr_, cap_, arr_ptr_, elem_type_) \
	do \
	{ \
		unsigned int len = len_expr_; \
		if (len >= cap_) \
		{ \
			unsigned int new_cap = umax(len, cap_ / 2 + 4); \
			elem_type_* new_array = realloc(arr_ptr_, \
				new_cap * sizeof(elem_type_)); \
			assert(new_array != NULL); \
			arr_ptr_ = new_array; \
			cap_ = new_cap; \
		} \
	} while (0)

#endif /* WHYCRYSTALS_HEADER_UTILS__ */
