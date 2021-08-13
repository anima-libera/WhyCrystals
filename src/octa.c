
#include "octa.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

void flags_col_givetoshader(GLuint attrib_index)
{
	glVertexAttribIPointer(attrib_index, 1, GL_UNSIGNED_INT,
		sizeof(flags_t), (void*)offsetof(flags_t, plain));
}

void pos_col_givetoshader(GLuint attrib_index)
{
	glVertexAttribPointer(attrib_index, 3, GL_FLOAT, GL_FALSE,
		sizeof(pos_t), (void*)offsetof(pos_t, x));
}

void color_col_givetoshader(GLuint attrib_index)
{
	glVertexAttribPointer(attrib_index, 3, GL_FLOAT, GL_FALSE,
		sizeof(color_t), (void*)offsetof(color_t, r));
}

const porp_info_t g_prop_info_table[PROP_TYPE_COUNT] = {
	[PTI_FLAGS] = FLAGS_INFO,
	[PTI_POS] = POS_INFO,
	[PTI_COLOR] = COLOR_INFO,
};

void pti_print(pti_t pti)
{
	assert(pti < PROP_TYPE_COUNT);
	printf("%s", g_prop_info_table[pti].name);
}

void ptis_print(const ptis_t* ptis)
{
	printf("[");
	for (unsigned int i = 0; i < ptis->len; i++)
	{
		pti_print(ptis->arr[i]);
		if (i != ptis->len - 1)
		{
			printf(",");
		}
	}
	printf("]");
}

colt_t* colt_alloc(const ptis_t* ptis)
{
	colt_t* colt = malloc(sizeof(colt_t) + sizeof(col_t) * ptis->len);
	colt->col_count = ptis->len;
	for (unsigned int i = 0; i < colt->col_count; i++)
	{
		colt->col_arr[i].pti = ptis->arr[i];
		glGenBuffers(1, &colt->col_arr[i].opengl_buf_id);
		colt->col_arr[i].data = NULL;
	}
	colt->row_count = 0;
	return colt;
}

void colt_add_rows(colt_t* colt, unsigned int how_much)
{
	unsigned int new_row_count = colt->row_count + how_much;
	for (unsigned int i = 0; i < colt->col_count; i++)
	{
		unsigned int prop_size = g_prop_info_table[colt->col_arr[i].pti].size;
		colt->col_arr[i].data = realloc(colt->col_arr[i].data,
			new_row_count * prop_size);
		glBindBuffer(GL_ARRAY_BUFFER, colt->col_arr[i].opengl_buf_id);
		glBufferData(GL_ARRAY_BUFFER, new_row_count * prop_size,
			colt->col_arr[i].data, GL_DYNAMIC_DRAW);
	}
	assert(colt->col_arr[0].pti == PTI_FLAGS);
	for (unsigned int i = colt->row_count; i < new_row_count; i++)
	{
		((flags_t*)colt->col_arr[0].data)[i].bit_set.exists = 0;
	}
	colt->row_count = new_row_count;
}

static int colt_is_row_used(const colt_t* colt, unsigned int row_index)
{
	assert(colt->col_arr[0].pti == PTI_FLAGS);
	return ((flags_t*)colt->col_arr[0].data)[row_index].bit_set.exists;
}

oi_t colt_alloc_obj(colt_t* colt)
{
	for (unsigned int i = 0; i < colt->row_count; i++)
	{
		if (!colt_is_row_used(colt, i))
		{
			return (oi_t){.colt = colt, .row_index = i};
		}
	}
	unsigned int first_new_index = colt->row_count;
	colt_add_rows(colt, colt->row_count / 2 + 4);
	return (oi_t){.colt = colt, .row_index = first_new_index};
}

void colt_print(const colt_t* colt)
{
	printf("ptis:");
	printf("[");
	for (unsigned int i = 0; i < colt->col_count; i++)
	{
		pti_print(colt->col_arr[i].pti);
		if (i != colt->col_count - 1)
		{
			printf(",");
		}
	}
	printf("]");
	printf(",");
	printf("glids:");
	printf("[");
	for (unsigned int i = 0; i < colt->col_count; i++)
	{
		printf("%u", colt->col_arr[i].opengl_buf_id);
		if (i != colt->col_count - 1)
		{
			printf(",");
		}
	}
	printf("]");
	printf(",");
	printf("rowcount:%u", colt->row_count);
	printf(",");
	printf("used:");
	printf("[");
	int is_first = 1;
	for (unsigned int i = 0; i < colt->row_count; i++)
	{
		if (colt_is_row_used(colt, i))
		{
			if (is_first)
			{
				is_first = 0;
			}
			else
			{
				printf(",");
			}
			printf("%u", i);
		}
	}
	printf("]");
}

void* oi_get_prop(oi_t oi, pti_t pti)
{
	unsigned int col_index = UINT_MAX;
	for (unsigned int i = 0; i < oi.colt->col_count; i++)
	{
		if (oi.colt->col_arr[i].pti == pti)
		{
			col_index = i;
			break;
		}
	}
	assert(col_index != UINT_MAX);
	char* data = oi.colt->col_arr[col_index].data;
	unsigned int prop_size = g_prop_info_table[pti].size;
	return data + oi.row_index * prop_size;
}

#if 0

octa_t g_octa = {0};

void pti_print(pti_t pti)
{
	assert(pti < PROP_TYPE_COUNT);
	fputs(g_prop_info_table[pti].name, stdout);
}

void ptis_add(ptis_t* ptis, pti_t pti)
{
	if (ptis->len == 0)
	{
		ptis->arr = malloc(sizeof(pti_t));
		ptis->arr[0] = pti;
		ptis->len = 1;
		return;
	}
	unsigned int new_len = ptis->len + 1;
	ptis->arr = realloc(ptis->arr, new_len * sizeof(pti_t));
	/* As arr should always be sorted, the good spot is searched for from the
	 * right and ptis greater than the new one are shifted to the right. */
	unsigned int i = ptis->len - 1;
	while (1)
	{
		if (ptis->arr[i] < pti)
		{
			ptis->arr[i+1] = pti;
			break;
		}
		else if (i == 0) /* && ptis->arr[i] >= pti */
		{
			assert(ptis->arr[i] != pti);
			ptis->arr[i+1] = ptis->arr[i];
			ptis->arr[i] = pti;
			break;
		}
		else
		{
			assert(ptis->arr[i] != pti);
			ptis->arr[i+1] = ptis->arr[i];
			i--;
		}
	}
	ptis->len = new_len;
}

void ptis_copy(const ptis_t* src, ptis_t* dst)
{
	dst->arr = malloc(src->len * sizeof(pti_t));
	memcpy(dst->arr, src->arr, src->len * sizeof(pti_t));
	dst->len = src->len;
}

int pti_eq(const ptis_t* a, const ptis_t* b)
{
	if (a->len != b->len)
	{
		return 0;
	}
	for (unsigned int i = 0; i < a->len; i++)
	{
		if (a->arr[i] != b->arr[i])
		{
			return 0;
		}
	}
	return 1;
}

void ptis_print(const ptis_t* ptis)
{
	fputs("ptis", stdout);
	fputs("[", stdout);
	for (unsigned int i = 0; i < ptis->len; i++)
	{
		pti_print(ptis->arr[i]);
		if (i != ptis->len - 1)
		{
			fputs(",", stdout);
		}
	}
	fputs("]", stdout);
}

void colt_init(colt_t* colt, const ptis_t* ptis)
{
	colt->row_count = 0;
	ptis_copy(ptis, &colt->ptis);
	colt->col_data_arr = calloc(ptis->len, sizeof(void*));
}

void colt_lengthen(colt_t* colt, unsigned int by_how_much)
{
	unsigned int new_row_count = colt->row_count + by_how_much;
	for (unsigned int i = 0; i < colt->ptis.len; i++)
	{
		unsigned int prop_size = g_prop_info_table[colt->ptis.arr[i]].size;
		colt->col_data_arr[i] = realloc(colt->col_data_arr[i],
			new_row_count * prop_size);
	}
	assert(colt->ptis.arr[0] == PTI_FLAGS);
	for (unsigned int i = colt->row_count; i < new_row_count; i++)
	{
		((flags_t*)colt->col_data_arr[0])[i].bit_set.exists = 0;
	}
	colt->row_count = new_row_count;
}

int colt_does_obj_exist(const colt_t* colt, unsigned int row_index)
{
	assert(colt->ptis.arr[0] == PTI_FLAGS);
	return ((flags_t*)colt->col_data_arr[0])[row_index].bit_set.exists;
}

void colt_print(const colt_t* colt)
{
	fputs("colt", stdout);
	fputs("(", stdout);
	printf("row_count:%d", colt->row_count);
	fputs(",", stdout);
	ptis_print(&colt->ptis);
	fputs(",", stdout);
	assert(colt->ptis.arr[0] == PTI_FLAGS);
	fputs("[", stdout);
	for (unsigned int i = 0; i < colt->row_count; i++)
	{
		fputs(colt_does_obj_exist(colt, i) ? "!" : "_", stdout);
	}
	fputs("]", stdout);
	fputs(")", stdout);
}

unsigned int octa_add_colt(const ptis_t* ptis)
{
	g_octa.len++;
	g_octa.colt_arr = realloc(g_octa.colt_arr, g_octa.len * sizeof(colt_t));
	unsigned int index = g_octa.len - 1;
	colt_init(&g_octa.colt_arr[index], ptis);
	return index;
}

oi_t octa_alloc_obj(const ptis_t* ptis)
{
	colt_t* colt = NULL;
	unsigned int colt_index;
	for (unsigned int i = 0; i < g_octa.len; i++)
	{
		if (pti_eq(&g_octa.colt_arr[i].ptis, ptis))
		{
			colt = &g_octa.colt_arr[i];
			colt_index = i;
			break;
		}
	}
	if (colt == NULL)
	{
		unsigned int i = octa_add_colt(ptis);
		colt = &g_octa.colt_arr[i];
		colt_index = i;
	}
	for (unsigned int i = 0; i < colt->row_count; i++)
	{
		if (!colt_does_obj_exist(colt, i))
		{
			return (oi_t){.colt_index = colt_index, .row_index = i};
		}
	}
	unsigned int old_row_count = colt->row_count;
	colt_lengthen(colt, colt->row_count / 2 + 4);
	return (oi_t){.colt_index = colt_index, .row_index = old_row_count};
}

void* octa_get_obj_prop(oi_t oi, pti_t pti)
{
	assert(oi.colt_index < g_octa.len);
	colt_t* colt = &g_octa.colt_arr[oi.colt_index];
	assert(oi.row_index < colt->row_count);
	unsigned int i;
	for (i = 0; i < colt->ptis.len; i++)
	{
		if (colt->ptis.arr[i] == pti)
		{
			break;
		}
	}
	assert(i < colt->ptis.len);
	void* col_data = colt->col_data_arr[i];
	unsigned int prop_size = g_prop_info_table[colt->ptis.arr[i]].size;
	return (char*)col_data + oi.row_index * prop_size;
}

void octa_print(void)
{
	fputs("octa", stdout);
	fputs("(", stdout);
	printf("len:%d", g_octa.len);
	fputs(",", stdout);
	fputs("[", stdout);
	for (unsigned int i = 0; i < g_octa.len; i++)
	{
		colt_print(&g_octa.colt_arr[i]);
		if (i != g_octa.len - 1)
		{
			fputs(",", stdout);
		}
	}
	fputs("]", stdout);
	fputs(")", stdout);
}

#endif
