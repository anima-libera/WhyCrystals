
#include "window.h"
#include "shaders.h"
#include "random.h"
#include "octa.h"
#include "spw.h"
#include "smata.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strcmp, memset */
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

		rg_t* g_rg = malloc(sizeof(rg_t));
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
	glEnable(GL_DEPTH_TEST);

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
	swp_update_window_wh(window_width, window_height);

	rg_t* g_rg = malloc(sizeof(rg_t));
	rg_time_seed(g_rg);

	if (init_smata() != 0)
	{
		return -1;
	}

	canvas_t canvas;
	canvas.w = 50;
	canvas.h = 50;
	canvas.data = calloc(canvas.w * canvas.h, sizeof(pixel_t));

	canvas.incanvas_sprite_rect.x = 0;
	canvas.incanvas_sprite_rect.y = 0;
	canvas.incanvas_sprite_rect.w = 8;
	canvas.incanvas_sprite_rect.h = 8;
	canvas.incanvas_sprite_rect.origin_x = 0.5f;
	canvas.incanvas_sprite_rect.origin_y = 0.5f;
	canvas.incanvas_sprite_rect.flags.bit_set.vertical = 1;

	for (unsigned int x = 0; x < canvas.incanvas_sprite_rect.w; x++)
	for (unsigned int y = 0; y < canvas.incanvas_sprite_rect.h; y++)
	{
		canvas.data[x + canvas.w * y] = (pixel_t){
			.r = rg_int(g_rg, 100, 255),
			.g = rg_int(g_rg, 0, 255),
			.b = rg_int(g_rg, 0, 100),
			.a = 255
		};
	}

	unsigned int sprite_id_test01 = smata_register_sprite(&canvas);

	unsigned int tree_sprite_number = rg_int(g_rg, 1, 6);
	unsigned int tree_sprite_id_arr[tree_sprite_number];

	for (unsigned int i = 0; i < tree_sprite_number; i++)
	{
		memset(canvas.data, 0, canvas.w * canvas.h * sizeof(pixel_t));

		unsigned int w = rg_int(g_rg, 1, 3) * 2 + 1;
		unsigned int h_bottom = rg_int(g_rg, 2, 6);
		unsigned int h_top = w < 5 ? w : rg_int(g_rg, 0, 3) == 0 ? (w-1) : w;
		int has_plus_shape = w >= 5 ? rg_int(g_rg, 0, 2) == 0 : 0;

		pixel_t bottom_color = {
			.r = rg_int(g_rg, 70, 255),
			.g = rg_int(g_rg, 0, 100),
			.b = rg_int(g_rg, 0, 70),
			.a = 255,
		};
		pixel_t top_color = {
			.r = rg_int(g_rg, 0, 200),
			.g = rg_int(g_rg, 80, 255),
			.b = rg_int(g_rg, 0, 100),
			.a = 255,
		};

		canvas.incanvas_sprite_rect.x = 0;
		canvas.incanvas_sprite_rect.y = 0;
		canvas.incanvas_sprite_rect.w = w;
		canvas.incanvas_sprite_rect.h = h_bottom + h_top;
		canvas.incanvas_sprite_rect.origin_x = 0.5f;
		canvas.incanvas_sprite_rect.origin_y = 0.0f;
		canvas.incanvas_sprite_rect.flags.bit_set.vertical = 1;

		for (unsigned int y = h_top; y < h_bottom + h_top; y++)
		{
			unsigned int x = w / 2;
			canvas.data[x + canvas.w * y] = bottom_color;
		}
		for (unsigned int x = 0; x < w; x++)
		for (unsigned int y = 0; y < h_top; y++)
		{
			canvas.data[x + canvas.w * y] = top_color;
		}
		if (has_plus_shape)
		{
			unsigned int x, y;
			x = 0;
			y = 0;
			canvas.data[x + canvas.w * y] = (pixel_t){0};
			x = w-1;
			y = 0;
			canvas.data[x + canvas.w * y] = (pixel_t){0};
			x = 0;
			y = h_top-1;
			canvas.data[x + canvas.w * y] = (pixel_t){0};
			x = w-1;
			y = h_top-1;
			canvas.data[x + canvas.w * y] = (pixel_t){0};
		}

		tree_sprite_id_arr[i] = smata_register_sprite(&canvas);
	}

	ptis_t* tree_ptis;
	PTIS_ALLOC_SET(tree_ptis, PTI_FLAGS, PTI_POS, PTI_SPRITEID, PTI_SCALE);
	ptis_print(tree_ptis); printf("\n");

	colt_t* tree_colt = colt_alloc(tree_ptis);
	colt_print(tree_colt); printf("\n");

	for (unsigned int i = 0; i < 40; i++)
	{
		oi_t oi = colt_alloc_obj(tree_colt);

		pos_t* pos = oi_get_prop(oi, PTI_POS);
		*pos = (pos_t){
			.x = rg_float(g_rg, -6.5f, 6.5f),
			.y = rg_float(g_rg, -3.0f, 3.0f),
			.z = 0.0f
		};

		spriteid_t* spriteid = oi_get_prop(oi, PTI_SPRITEID);
		spriteid->sprite_id =
			tree_sprite_id_arr[rg_int(g_rg, 0, tree_sprite_number-1)];

		scale_t* scale = oi_get_prop(oi, PTI_SCALE);
		scale->scale = 1.0f;
	}

	colt_print(tree_colt); printf("\n");

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
					}
				break;
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//swp_apply_on_colt(SPW_ID_POS, tree_colt);
		swp_apply_on_colt(SPW_ID_SPRITE, tree_colt);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
