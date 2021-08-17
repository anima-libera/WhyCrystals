
#include "window.h"
#include "shaders.h"
#include "random.h"
#if 0
#include "da.h"
#include "world.h"
#include "visuals.h"
#endif
#include "octa.h"
#include "spw.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strcmp */
#include <math.h>
#include <assert.h>

#ifdef cplusplus__
#error C compiler required
#endif

#define TAU 6.28318530717

int main(int argc, char** argv)
{
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
		size_t write(int fd, const void* buf, size_t count);

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

	if (init_swp_table() != 0)
	{
		return -1;
	}

	int window_width, window_height;
	SDL_GL_GetDrawableSize(g_window, &window_width, &window_height);
	//glProgramUniform2ui(g_shprog_draw_visuals, 1, window_width, window_height);
	//glProgramUniform2ui(g_shprog_draw_pos, 1, window_width, window_height);
	swp_update_window_wh(window_width, window_height);

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

	ptis_t* ptis;
	PTIS_ALLOC_SET(ptis, PTI_FLAGS, PTI_POS, PTI_SPRITEID);
	ptis_print(ptis); printf("\n");

	colt_t* colt = colt_alloc(ptis);
	colt_print(colt); printf("\n");
	oi_t oi = colt_alloc_obj(colt);
	pos_t* pos = oi_get_prop(oi, PTI_POS);
	*pos = (pos_t){.x = 0.0f, .y = 0.0f, .z = 0.0f};
	colt_print(colt); printf("\n");
	oi = colt_alloc_obj(colt);
	pos = oi_get_prop(oi, PTI_POS);
	*pos = (pos_t){.x = 3.0f, .y = 1.0f, .z = 0.0f};
	colt_print(colt); printf("\n");
	for (unsigned int i = 0; i < 17 - 2; i++)
	{
		oi = colt_alloc_obj(colt);
		pos = oi_get_prop(oi, PTI_POS);
		*pos = (pos_t){
			.x = rg_float(g_rg, 4.5f, 6.5f),
			.y = rg_float(g_rg, -2.0f, 2.0f),
			.z = 0.0f
		};
	}
	colt_print(colt); printf("\n");
	flags_t* flags;
	flags = oi_get_prop((oi_t){.colt = colt, .row_index = 9}, PTI_FLAGS);
	flags->bit_set.exists = 0;
	flags = oi_get_prop((oi_t){.colt = colt, .row_index = 11}, PTI_FLAGS);
	flags->bit_set.exists = 0;
	colt_print(colt); printf("\n");

	//return 0;

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

		swp_apply_on_colt(SPW_ID_POS, colt);
		swp_apply_on_colt(SPW_ID_SPRITES, colt);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
