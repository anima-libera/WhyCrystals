
#ifndef WHYCRYSTALS_HEADER_UTILS__
#define WHYCRYSTALS_HEADER_UTILS__

#include <stdlib.h>
#include <assert.h>

unsigned int umax(unsigned int a, unsigned int b);

/* Dynamic array reallocator.
 * There is no need for such things as templates or high-level features,
 * because C has genericity support (proof here).
 * To use this macro for a dynamic array such as
 *  struct {
 *   unsigned int len;
 *   unsigned int cap;
 *   type_contained_t* arr;
 *  } da = {0};
 * then the indented invocation (for, say, add one cell) should be
 *  DA_LENGTHEN(da.len += 1, da.cap, da.arr, type_contained_t);
 * and new cells (in this case, one new cell) will be allocated at the end
 * of the array, and filled with garbage values (beware!). */
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
