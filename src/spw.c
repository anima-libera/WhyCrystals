
#include "spw.h"
#include "shaders.h"
#include "octa.h"
#include "smata.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

spw_t g_spw_table[SPW_COUNT] = {0};

int init_spw_table(void)
{
	#define UNIFORM(whichone_, location_) \
		((spw_uniform_t){.whichone = whichone_, .location = location_})
	#define SPW_DECLARE_UNIFORMS(index_, ...) \
		do \
		{ \
			const spw_uniform_t uniform_array[] = {__VA_ARGS__}; \
			g_spw_table[index_].uniform_arr = malloc(sizeof(uniform_array)); \
			g_spw_table[index_].uniform_count = \
				sizeof(uniform_array) / sizeof(spw_uniform_t); \
			memcpy(g_spw_table[index_].uniform_arr, uniform_array, \
				sizeof(uniform_array)); \
		} while (0)

	#define BUFFER(whichone_, binding_) \
		((spw_buffer_t){.whichone = whichone_, .binding = binding_})
	#define SPW_DECLARE_BUFFERS(index_, ...) \
		do \
		{ \
			const spw_buffer_t buffer_array[] = {__VA_ARGS__}; \
			g_spw_table[index_].buffer_arr = malloc(sizeof(buffer_array)); \
			g_spw_table[index_].buffer_count = \
				sizeof(buffer_array) / sizeof(spw_buffer_t); \
			memcpy(g_spw_table[index_].buffer_arr, buffer_array, \
				sizeof(buffer_array)); \
		} while (0)

	#define ATTRIB_SET(pti_, ...) \
		((spw_attrib_set_t){ \
			.pti = pti_, \
			.location_count = ARGS_COUNT(__VA_ARGS__), \
			.location_arr = ARGS_ALLOCATED(GLuint, __VA_ARGS__)})
	#define SPW_DECLARE_ATTRIB_SETS(index_, ...) \
		do \
		{ \
			const spw_attrib_set_t attrib_set_array[] = {__VA_ARGS__}; \
			g_spw_table[index_].attrib_set_arr = malloc(sizeof(attrib_set_array)); \
			g_spw_table[index_].attrib_set_count = \
				sizeof(attrib_set_array) / sizeof(spw_attrib_set_t); \
			memcpy(g_spw_table[index_].attrib_set_arr, attrib_set_array, \
				sizeof(attrib_set_array)); \
		} while (0)

	{
		unsigned int spw_id = SPW_ID_POS;
		g_spw_table[spw_id].shprog_id = g_shprog_draw_pos;
		SPW_DECLARE_UNIFORMS(spw_id,
			UNIFORM(U_WINDOW_WH, 1));
		SPW_DECLARE_ATTRIB_SETS(spw_id,
			ATTRIB_SET(PTI_FLAGS, 0),
			ATTRIB_SET(PTI_POS, 1));
	}
	{
		unsigned int spw_id = SPW_ID_SPRITE;
		g_spw_table[spw_id].shprog_id = g_shprog_draw_sprite;
		SPW_DECLARE_UNIFORMS(spw_id,
			UNIFORM(U_ATLAS, 0),
			UNIFORM(U_WINDOW_WH, 1));
		SPW_DECLARE_BUFFERS(spw_id,
			BUFFER(B_SMATA_SPRITE_RECTS, 0));
		SPW_DECLARE_ATTRIB_SETS(spw_id,
			ATTRIB_SET(PTI_FLAGS, 0),
			ATTRIB_SET(PTI_POS, 1),
			ATTRIB_SET(PTI_SPRITE, 2, 3));
	}

	#undef UNIFORM
	#undef SPW_DECLARE_UNIFORMS
	#undef BUFFER
	#undef SPW_DECLARE_BUFFERS
	#undef ATTRIB_SET
	#undef SPW_DECLARE_ATTRIB_SETS

	return 0;
}

static GLuint get_buffer_openglid(spw_buffer_whichone_t whichone)
{
	switch (whichone)
	{
		case B_SMATA_SPRITE_RECTS:
			return g_smata.sr_buffer_opengl_id;
		default:
			assert(0);
	}
}

void spw_apply_on_colt(spw_id_t spw_id, colt_t* colt)
{
	glUseProgram(g_spw_table[spw_id].shprog_id);

	for (unsigned int i = 0; i < g_spw_table[spw_id].attrib_set_count; i++)
	{
		const spw_attrib_set_t* attrib_set = &g_spw_table[spw_id].attrib_set_arr[i];
		for (unsigned int j = 0; j < attrib_set->location_count; j++)
		{
			glEnableVertexAttribArray(attrib_set->location_arr[j]);
		}
	}

	for (unsigned int i = 0; i < g_spw_table[spw_id].attrib_set_count; i++)
	{
		const spw_attrib_set_t* attrib_set = &g_spw_table[spw_id].attrib_set_arr[i];
		col_t* col = colt_get_col(colt, attrib_set->pti);
		if (col->opengl_buf_id == 0)
		{
			glGenBuffers(1, &col->opengl_buf_id);
			glBindBuffer(GL_ARRAY_BUFFER, col->opengl_buf_id);
			glBufferData(GL_ARRAY_BUFFER,
				colt->row_count * g_prop_info_table[attrib_set->pti].size,
				col->data,
				GL_DYNAMIC_DRAW);
		}
		else if (col->opengl_buf_needs_resize)
		{
			glBindBuffer(GL_ARRAY_BUFFER, col->opengl_buf_id);
			glBufferData(GL_ARRAY_BUFFER,
				colt->row_count * g_prop_info_table[attrib_set->pti].size,
				col->data,
				GL_DYNAMIC_DRAW);
			col->opengl_buf_needs_resize = 0;
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, col->opengl_buf_id);
			glBufferSubData(GL_ARRAY_BUFFER, 0,
				colt->row_count * g_prop_info_table[attrib_set->pti].size,
				col->data);
		}
		assert(g_prop_info_table[attrib_set->pti].attrib_count ==
			attrib_set->location_count);
		g_prop_info_table[attrib_set->pti].col_givetoshader_callback(
			attrib_set->location_arr);
	}

	for (unsigned int i = 0; i < g_spw_table[spw_id].buffer_count; i++)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
			g_spw_table[spw_id].buffer_arr[i].binding,
			get_buffer_openglid(g_spw_table[spw_id].buffer_arr[i].whichone));
	}

	glDrawArrays(GL_POINTS, 0, colt->row_count);

	for (unsigned int i = 0; i < g_spw_table[spw_id].attrib_set_count; i++)
	{
		const spw_attrib_set_t* attrib_set = &g_spw_table[spw_id].attrib_set_arr[i];
		for (unsigned int j = 0; j < attrib_set->location_count; j++)
		{
			glDisableVertexAttribArray(attrib_set->location_arr[j]);
		}
	}

	glUseProgram((GLuint)0);
}

void spw_update_atlas(unsigned int texture_unit_id)
{
	for (unsigned int i = 0; i < SPW_COUNT; i++)
	{
		for (unsigned int j = 0; j < g_spw_table[i].uniform_count; j++)
		{
			if (g_spw_table[i].uniform_arr[j].whichone == U_ATLAS)
			{
				glProgramUniform1i(g_spw_table[i].shprog_id,
					g_spw_table[i].uniform_arr[j].location,
					texture_unit_id);
				break;
			}
		}
	}
}

void spw_update_window_wh(unsigned int w, unsigned int h)
{
	for (unsigned int i = 0; i < SPW_COUNT; i++)
	{
		for (unsigned int j = 0; j < g_spw_table[i].uniform_count; j++)
		{
			if (g_spw_table[i].uniform_arr[j].whichone == U_WINDOW_WH)
			{
				glProgramUniform2ui(g_spw_table[i].shprog_id,
					g_spw_table[i].uniform_arr[j].location,
					w, h);
				break;
			}
		}
	}
}
