
#include "octa.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

void flags_col_givetoshader(GLuint* attrib_location_arr)
{
	glVertexAttribIPointer(attrib_location_arr[0], 1, GL_UNSIGNED_INT,
		sizeof(flags_t), (void*)offsetof(flags_t, plain));
}

void pos_col_givetoshader(GLuint* attrib_location_arr)
{
	glVertexAttribPointer(attrib_location_arr[0], 3, GL_FLOAT, GL_FALSE,
		sizeof(pos_t), (void*)offsetof(pos_t, x));
}

void sprite_col_givetoshader(GLuint* attrib_location_arr)
{
	glVertexAttribIPointer(attrib_location_arr[0], 1, GL_UNSIGNED_INT,
		sizeof(sprite_t), (void*)offsetof(sprite_t, sprite_id));
	glVertexAttribPointer(attrib_location_arr[1], 1, GL_FLOAT, GL_FALSE,
		sizeof(sprite_t), (void*)offsetof(sprite_t, scale));
}

void color_col_givetoshader(GLuint* attrib_location_arr)
{
	glVertexAttribPointer(attrib_location_arr[0], 3, GL_FLOAT, GL_FALSE,
		sizeof(color_t), (void*)offsetof(color_t, r));
}

const porp_info_t g_prop_info_table[PROP_TYPE_COUNT] = {
	[PTI_FLAGS] = FLAGS_INFO,
	[PTI_POS] = POS_INFO,
	[PTI_SPRITE] = SPRITE_INFO,
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

static void colt_set_row_used_flag(const colt_t* colt, unsigned int row_index,
	int flag)
{
	assert(colt->col_arr[0].pti == PTI_FLAGS);
	assert(flag == 0 || flag == 1);
	((flags_t*)colt->col_arr[0].data)[row_index].bit_set.exists = flag;
}

static int colt_is_row_used(const colt_t* colt, unsigned int row_index)
{
	assert(colt->col_arr[0].pti == PTI_FLAGS);
	return ((flags_t*)colt->col_arr[0].data)[row_index].bit_set.exists;
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
		colt_set_row_used_flag(colt, i, 0);
	}
	colt->row_count = new_row_count;
}

oi_t colt_alloc_obj(colt_t* colt)
{
	for (unsigned int i = 0; i < colt->row_count; i++)
	{
		if (!colt_is_row_used(colt, i))
		{
			colt_set_row_used_flag(colt, i, 1);
			return (oi_t){.colt = colt, .row_index = i};
		}
	}
	unsigned int first_new_index = colt->row_count;
	colt_add_rows(colt, colt->row_count / 2 + 4);
	colt_set_row_used_flag(colt, first_new_index, 1);
	return (oi_t){.colt = colt, .row_index = first_new_index};
}

const col_t* colt_get_col(const colt_t* colt, pti_t pti)
{
	unsigned int col_index = UINT_MAX;
	for (unsigned int i = 0; i < colt->col_count; i++)
	{
		if (colt->col_arr[i].pti == pti)
		{
			col_index = i;
			break;
		}
	}
	assert(col_index != UINT_MAX);
	return &colt->col_arr[col_index];
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
	int is_first_printed = 1;
	unsigned int row_index = 0;
	while (1)
	{
		while (row_index < colt->row_count && !colt_is_row_used(colt, row_index))
		{
			row_index++;
		}
		if (!(row_index < colt->row_count))
		{
			break;
		}
		unsigned int interval_start = row_index;
		while (row_index < colt->row_count && colt_is_row_used(colt, row_index))
		{
			row_index++;
		}
		unsigned int interval_end = row_index - 1;
		if (is_first_printed)
		{
			is_first_printed = 0;
		}
		else
		{
			printf(",");
		}
		if (interval_start == interval_end)
		{
			printf("%u", interval_start);
		}
		else
		{
			printf("%u~%u", interval_start, interval_end);
		}
	}
	printf("]");
}

void* obj_get_prop(oi_t oi, pti_t pti)
{
	char* data = colt_get_col(oi.colt, pti)->data;
	unsigned int prop_size = g_prop_info_table[pti].size;
	return data + oi.row_index * prop_size;
}
