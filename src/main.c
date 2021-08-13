
#include "window.h"
#include "shaders.h"
#include "random.h"
#if 0
#include "da.h"
#include "world.h"
#include "visuals.h"
#endif
#include "octa.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strcmp */
#include <math.h>
#include <assert.h>

size_t write(int fd, const void* buf, size_t count);

#define TAU 6.28318530717

int main(int argc, char** argv)
{








	#if 0
	ptis_t ptis_a = {0};
	ptis_add(&ptis_a, PTI_FLAGS);
	ptis_add(&ptis_a, PTI_COLOR);
	ptis_add(&ptis_a, PTI_POS);
	//ptis_print(&ptis_a); fputs("\n", stdout);
	//return 0;

	octa_print(); fputs("\n", stdout);
	oi_t oi = octa_alloc_obj(&ptis_a);
	octa_print(); fputs("\n", stdout);
	flags_t* flags = octa_get_obj_prop(oi, PTI_FLAGS);
	flags->bit_set.exists = 1;
	pos_t* pos = octa_get_obj_prop(oi, PTI_POS);
	pos->x = 0.0f;
	pos->y = 0.0f;
	pos->z = 0.0f;
	octa_print(); fputs("\n", stdout);
	oi = octa_alloc_obj(&ptis_a);
	flags = octa_get_obj_prop(oi, PTI_FLAGS);
	flags->bit_set.exists = 1;
	pos = octa_get_obj_prop(oi, PTI_POS);
	pos->x = 3.0f;
	pos->y = 1.0f;
	pos->z = 0.0f;
	octa_print(); fputs("\n", stdout);

	ptis_t ptis_b = {0};
	ptis_add(&ptis_b, PTI_FLAGS);
	ptis_add(&ptis_b, PTI_POS);
	oi = octa_alloc_obj(&ptis_b);
	flags = octa_get_obj_prop(oi, PTI_FLAGS);
	flags->bit_set.exists = 1;
	pos = octa_get_obj_prop(oi, PTI_POS);
	pos->x = 4.0f;
	pos->y = 4.0f;
	pos->z = 0.0f;
	octa_print(); fputs("\n", stdout);

	//return 0;
	#endif



	int test_randon_generator = 0;
	for (unsigned int i = 1; i < (unsigned int)argc; i++)
	{
		if (strcmp(argv[i], "-r") == 0)
		{
			test_randon_generator = 1;
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			fputs("bruh\n", stdout);
		}
	}

	if (test_randon_generator)
	{
		g_rg = malloc(sizeof(rg_t));
		rg_time_seed(g_rg);
		uint32_t buf32;
		while (1)
		{
			buf32 = rg_uint32_full(g_rg);
			write(1, &buf32, 4);
		}
		return 0;
	}

	if (init_graphics() != 0)
	{
		return -1;
	}

	if (shprog_build_all() != 0)
	{
		return -1;
	}

	int window_width, window_height;
	SDL_GL_GetDrawableSize(g_window, &window_width, &window_height);
	glProgramUniform2ui(g_shprog_draw_visuals, 1, window_width, window_height);
	glProgramUniform2ui(g_shprog_draw_pos, 1, window_width, window_height);

	g_rg = malloc(sizeof(rg_t));
	rg_time_seed(g_rg);

	glEnable(GL_DEPTH_TEST);

	#if 0
	generate_texture_map();

	world_t* world = malloc(sizeof(world_t));
	world_init(world);
	generate_world_map(world);
	generate_player(world);

	for (unsigned int i = 0; i < 6; i++)
	{
		generate_animal(world,
			rg_float(g_rg, -4.5f, 4.5f), rg_float(g_rg, -4.5f, 4.5f));
	}

	for (unsigned int i = 0; i < 10; i++)
	{
		generate_plant(world,
			rg_float(g_rg, -4.5f, 4.5f), rg_float(g_rg, -4.5f, 4.5f));
	}

	glGenBuffers(1, &world->buf_id_visuals);
	#endif














	printf("test\n");

	ptis_t* ptis;
	PTIS_ALLOC(ptis, PTI_FLAGS, PTI_POS);
	ptis_print(ptis); printf("\n");

	colt_t* colt = colt_alloc(ptis);
	colt_print(colt); printf("\n");
	oi_t oi = colt_alloc_obj(colt);
	colt_print(colt); printf("\n");
	flags_t* flags = oi_get_prop(oi, PTI_FLAGS);
	flags->bit_set.exists = 1;
	pos_t* pos = oi_get_prop(oi, PTI_POS);
	*pos = (pos_t){.x = 0.0f, .y = 0.0f, .z = 0.0f};
	colt_print(colt); printf("\n");
	oi = colt_alloc_obj(colt);
	flags = oi_get_prop(oi, PTI_FLAGS);
	flags->bit_set.exists = 1;
	pos = oi_get_prop(oi, PTI_POS);
	*pos = (pos_t){.x = 3.0f, .y = 1.0f, .z = 0.0f};
	colt_print(colt); printf("\n");

	//return 0;


















	GLuint flags_buf_id;
	GLuint pos_buf_id;

	glGenBuffers(1, &flags_buf_id);
	glGenBuffers(1, &pos_buf_id);

	#if 0
	glBindBuffer(GL_ARRAY_BUFFER, flags_buf_id);
	glBufferData(GL_ARRAY_BUFFER,
		4 * sizeof(flags_t),
		octa_get_obj_prop((oi_t){0, 0}, PTI_FLAGS), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, pos_buf_id);
	glBufferData(GL_ARRAY_BUFFER,
		4 * sizeof(pos_t),
		octa_get_obj_prop((oi_t){0, 0}, PTI_POS), GL_DYNAMIC_DRAW);
	#endif

	int running = 1;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = 0;
				break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = 0;
						break;
						#if 0
						case SDLK_UP:
							move_player(world, 0.0f, +0.05f);
						break;
						case SDLK_DOWN:
							move_player(world, 0.0f, -0.05f);
						break;
						case SDLK_RIGHT:
							move_player(world, +0.05f, 0.0f);
						break;
						case SDLK_LEFT:
							move_player(world, -0.05f, 0.0f);
						break;
						case SDLK_s:
							generate_player_shot(world);
						break;
						#endif
					}
				break;
			}
		}

		#if 0
		world_iter(world);

		if (world->visual_modified)
		{
			glBindBuffer(GL_ARRAY_BUFFER, world->buf_id_visuals);
			glBufferData(GL_ARRAY_BUFFER,
				world->visual_da.len * sizeof(visual_t),
				world->visual_da.arr, GL_DYNAMIC_DRAW);

			world->visual_modified = 0;
		}
		#endif

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#if 0
		glUseProgram(g_shprog_draw_visuals);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		
		glBindBuffer(GL_ARRAY_BUFFER, world->buf_id_visuals);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(visual_t),
			(void*)offsetof(visual_t, x));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(visual_t),
			(void*)offsetof(visual_t, w));
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE,
			sizeof(visual_t),
			(void*)(offsetof(visual_t, texture_rect) +
				offsetof(texture_rect_t, x)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			sizeof(visual_t),
			(void*)(offsetof(visual_t, texture_rect) +
				offsetof(texture_rect_t, origin_x)));
		glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT,
			sizeof(visual_t),
			(void*)offsetof(visual_t, flags));

		glDrawArrays(GL_POINTS, 0, world->visual_da.len);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glUseProgram((GLuint)0);
		#endif

		glUseProgram(g_shprog_draw_pos);
		for (unsigned int i = 0; i < 2; i++)
		{
			glEnableVertexAttribArray(i);
		}
		
		#if 0
		glBindBuffer(GL_ARRAY_BUFFER, flags_buf_id);
		flags_col_givetoshader(0);
		glBindBuffer(GL_ARRAY_BUFFER, pos_buf_id);
		pos_col_givetoshader(1);
		#endif

		unsigned int col_index = 0;
		unsigned int attrib_index = 0;
		pti_t pti = colt->col_arr[col_index].pti;
		glBindBuffer(GL_ARRAY_BUFFER, colt->col_arr[col_index].opengl_buf_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			colt->row_count * g_prop_info_table[pti].size,
			colt->col_arr[col_index].data);
		g_prop_info_table[pti].col_givetoshader_callback(attrib_index);
		
		col_index = 1;
		attrib_index = 1;
		pti = colt->col_arr[col_index].pti;
		glBindBuffer(GL_ARRAY_BUFFER, colt->col_arr[col_index].opengl_buf_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			colt->row_count * g_prop_info_table[pti].size,
			colt->col_arr[col_index].data);
		g_prop_info_table[pti].col_givetoshader_callback(attrib_index);

		glDrawArrays(GL_POINTS, 0, colt->row_count);

		for (unsigned int i = 0; i < 2; i++)
		{
			glDisableVertexAttribArray(i);
		}
		glUseProgram((GLuint)0);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
