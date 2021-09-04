
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

int init_swp_table(void)
{
	#define U(whichone_, location_) \
		((spw_uniform_t){.whichone = whichone_, .location = location_})
	#define SWP_DECLARE_UNIFORMS(index_, ...) \
		do \
		{ \
			const spw_uniform_t uniform_array[] = {__VA_ARGS__}; \
			g_spw_table[index_].uniform_arr = malloc(sizeof(uniform_array)); \
			g_spw_table[index_].uniform_count = \
				sizeof(uniform_array) / sizeof(spw_uniform_t); \
			memcpy(g_spw_table[index_].uniform_arr, uniform_array, \
				sizeof(uniform_array)); \
		} while (0)

	#define B(whichone_, binding_) \
		((spw_buffer_t){.whichone = whichone_, .binding = binding_})
	#define SWP_DECLARE_BUFFERS(index_, ...) \
		do \
		{ \
			const spw_buffer_t buffer_array[] = {__VA_ARGS__}; \
			g_spw_table[index_].buffer_arr = malloc(sizeof(buffer_array)); \
			g_spw_table[index_].buffer_count = \
				sizeof(buffer_array) / sizeof(spw_buffer_t); \
			memcpy(g_spw_table[index_].buffer_arr, buffer_array, \
				sizeof(buffer_array)); \
		} while (0)

	#define A(pti_, ...) \
		((spw_attrib_t){ \
			.pti = pti_, \
			.location_count = ARGS_COUNT(__VA_ARGS__), \
			.location_arr = ARGS_ALLOCATED(GLuint, __VA_ARGS__)})
	#define SWP_DECLARE_ATTRIBS(index_, ...) \
		do \
		{ \
			const spw_attrib_t attrib_array[] = {__VA_ARGS__}; \
			g_spw_table[index_].attrib_arr = malloc(sizeof(attrib_array)); \
			g_spw_table[index_].attrib_count = \
				sizeof(attrib_array) / sizeof(spw_attrib_t); \
			memcpy(g_spw_table[index_].attrib_arr, attrib_array, \
				sizeof(attrib_array)); \
		} while (0)

	{
		unsigned int swp_id = SPW_ID_POS;
		g_spw_table[swp_id].shprog_id = g_shprog_draw_pos;
		SWP_DECLARE_UNIFORMS(swp_id,
			U(U_WINDOW_WH, 1));
		SWP_DECLARE_ATTRIBS(swp_id,
			A(PTI_FLAGS, 0),
			A(PTI_POS, 1));
	}
	{
		unsigned int swp_id = SPW_ID_SPRITE;
		g_spw_table[swp_id].shprog_id = g_shprog_draw_sprite;
		SWP_DECLARE_UNIFORMS(swp_id,
			U(U_ATLAS, 0),
			U(U_WINDOW_WH, 1));
		SWP_DECLARE_BUFFERS(swp_id,
			B(B_SMATA_SPRITE_RECTS, 0));
		SWP_DECLARE_ATTRIBS(swp_id,
			A(PTI_FLAGS, 0),
			A(PTI_POS, 1),
			A(PTI_SPRITEID, 2),
			A(PTI_SCALE, 3));
	}

	#undef U
	#undef SWP_DECLARE_UNIFORMS
	#undef B
	#undef SWP_DECLARE_BUFFERS
	#undef A
	#undef SWP_DECLARE_ATTRIBS

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

void swp_apply_on_colt(spw_id_t spw_id, colt_t* colt)
{
	glUseProgram(g_spw_table[spw_id].shprog_id);

	for (unsigned int i = 0; i < g_spw_table[spw_id].attrib_count; i++)
	{
		glEnableVertexAttribArray(i);
	}
	for (unsigned int i = 0; i < g_spw_table[spw_id].attrib_count; i++)
	{
		const spw_attrib_t* attrib = &g_spw_table[spw_id].attrib_arr[i];
		const col_t* col = colt_get_col(colt, attrib->pti);
		GLuint opengl_buf_id = col->opengl_buf_id;
		glBindBuffer(GL_ARRAY_BUFFER, opengl_buf_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			colt->row_count * g_prop_info_table[attrib->pti].size,
			col->data);
		g_prop_info_table[attrib->pti].col_givetoshader_callback(
			attrib->location_arr);
	}

	for (unsigned int i = 0; i < g_spw_table[spw_id].buffer_count; i++)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
			g_spw_table[spw_id].buffer_arr[i].binding,
			get_buffer_openglid(g_spw_table[spw_id].buffer_arr[i].whichone));
	}

	glDrawArrays(GL_POINTS, 0, colt->row_count);

	for (unsigned int i = 0; i < g_spw_table[spw_id].attrib_count; i++)
	{
		glDisableVertexAttribArray(i);
	}

	glUseProgram((GLuint)0);
}

void swp_update_atlas(unsigned int texture_unit_id)
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

void swp_update_window_wh(unsigned int w, unsigned int h)
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
