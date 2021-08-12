
#include "obj.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

const porp_info_t g_prop_info_table[PROP_TYPE_COUNT] = {
	[PTI_FLAGS] = {.name = "flags", .size = sizeof(flags_t)},
	[PTI_POS] = {.name = "pos", .size = sizeof(pos_t)},
	[PTI_COLOR] = {.name = "color", .size = sizeof(color_t)},
};

octa_t g_octa = {0};

void pti_print(pti_t pti)
{
	assert(pti < PROP_TYPE_COUNT);
	fputs(g_prop_info_table[pti].name, stdout);
}

void pti_set_add(pti_set_t* pti_set, pti_t pti)
{
	if (pti_set->len == 0)
	{
		pti_set->arr = malloc(sizeof(pti_t));
		pti_set->arr[0] = pti;
		pti_set->len = 1;
		return;
	}
	unsigned int new_len = pti_set->len + 1;
	pti_set->arr = realloc(pti_set->arr, new_len * sizeof(pti_t));
	/* As arr should always be sorted, the good spot is searched for from the
	 * right and ptis greater than the new one are shifted to the right. */
	unsigned int i = pti_set->len - 1;
	while (1)
	{
		if (pti_set->arr[i] < pti)
		{
			pti_set->arr[i+1] = pti;
			break;
		}
		else if (i == 0) /* && pti_set->arr[i] >= pti */
		{
			assert(pti_set->arr[i] != pti);
			pti_set->arr[i+1] = pti_set->arr[i];
			pti_set->arr[i] = pti;
			break;
		}
		else
		{
			assert(pti_set->arr[i] != pti);
			pti_set->arr[i+1] = pti_set->arr[i];
			i--;
		}
	}
	pti_set->len = new_len;
}

void pti_set_copy(const pti_set_t* src, pti_set_t* dst)
{
	dst->arr = malloc(src->len * sizeof(pti_t));
	memcpy(dst->arr, src->arr, src->len * sizeof(pti_t));
	dst->len = src->len;
}

int pti_eq(const pti_set_t* a, const pti_set_t* b)
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

void pti_set_print(const pti_set_t* pti_set)
{
	fputs("pti_set", stdout);
	fputs("[", stdout);
	for (unsigned int i = 0; i < pti_set->len; i++)
	{
		pti_print(pti_set->arr[i]);
		if (i != pti_set->len - 1)
		{
			fputs(",", stdout);
		}
	}
	fputs("]", stdout);
}

void col_table_init(col_table_t* col_table, const pti_set_t* pti_set)
{
	col_table->col_len = 0;
	pti_set_copy(pti_set, &col_table->pti_set);
	col_table->col_data_arr = calloc(pti_set->len, sizeof(void*));
}

void col_table_lengthen(col_table_t* col_table, unsigned int by_how_much)
{
	unsigned int new_col_len = col_table->col_len + by_how_much;
	for (unsigned int i = 0; i < col_table->pti_set.len; i++)
	{
		unsigned int prop_size =
			g_prop_info_table[col_table->pti_set.arr[i]].size;
		col_table->col_data_arr[i] = realloc(col_table->col_data_arr[i],
			new_col_len * prop_size);
	}
	assert(col_table->pti_set.arr[0] == PTI_FLAGS);
	for (unsigned int i = col_table->col_len; i < new_col_len; i++)
	{
		((flags_t*)col_table->col_data_arr[0])[i].flags = 0;
	}
	col_table->col_len = new_col_len;
}

int col_table_does_obj_exist(const col_table_t* col_table, unsigned int row_index)
{
	assert(col_table->pti_set.arr[0] == PTI_FLAGS);
	unsigned int flags =
		((flags_t*)col_table->col_data_arr[0])[row_index].flags;
	return flags & OBJ_FLAG_EXISTS;
}

void col_table_print(const col_table_t* col_table)
{
	fputs("col_table", stdout);
	fputs("(", stdout);
	printf("col_len:%d", col_table->col_len);
	fputs(",", stdout);
	pti_set_print(&col_table->pti_set);
	fputs(",", stdout);
	assert(col_table->pti_set.arr[0] == PTI_FLAGS);
	fputs("[", stdout);
	for (unsigned int i = 0; i < col_table->col_len; i++)
	{
		fputs(col_table_does_obj_exist(col_table, i) ? "!" : "_", stdout);
	}
	fputs("]", stdout);
	fputs(")", stdout);
}

unsigned int octa_add_col_table(const pti_set_t* pti_set)
{
	g_octa.len++;
	g_octa.col_table_arr = realloc(g_octa.col_table_arr,
		g_octa.len * sizeof(col_table_t));
	unsigned int index = g_octa.len - 1;
	col_table_init(&g_octa.col_table_arr[index], pti_set);
	return index;
}

obj_index_t octa_alloc_obj(const pti_set_t* pti_set)
{
	col_table_t* col_table = NULL;
	unsigned int col_table_index;
	for (unsigned int i = 0; i < g_octa.len; i++)
	{
		if (pti_eq(&g_octa.col_table_arr[i].pti_set, pti_set))
		{
			col_table = &g_octa.col_table_arr[i];
			col_table_index = i;
			break;
		}
	}
	if (col_table == NULL)
	{
		unsigned int i = octa_add_col_table(pti_set);
		col_table = &g_octa.col_table_arr[i];
		col_table_index = i;
	}
	for (unsigned int i = 0; i < col_table->col_len; i++)
	{
		if (!col_table_does_obj_exist(col_table, i))
		{
			return (obj_index_t){
				.col_table_index = col_table_index,
				.row_index = i
			};
		}
	}
	unsigned int old_col_len = col_table->col_len;
	col_table_lengthen(col_table, col_table->col_len / 2 + 4);
	return (obj_index_t){
		.col_table_index = col_table_index,
		.row_index = old_col_len
	};
}

void* octa_get_obj_prop(obj_index_t oi, pti_t pti)
{
	assert(oi.col_table_index < g_octa.len);
	col_table_t* col_table = &g_octa.col_table_arr[oi.col_table_index];
	assert(oi.row_index < col_table->col_len);
	unsigned int i;
	for (i = 0; i < col_table->pti_set.len; i++)
	{
		if (col_table->pti_set.arr[i] == pti)
		{
			break;
		}
	}
	assert(i < col_table->pti_set.len);
	void* col_data = col_table->col_data_arr[i];
	unsigned int prop_size = g_prop_info_table[col_table->pti_set.arr[i]].size;
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
		col_table_print(&g_octa.col_table_arr[i]);
		if (i != g_octa.len - 1)
		{
			fputs(",", stdout);
		}
	}
	fputs("]", stdout);
	fputs(")", stdout);
}
