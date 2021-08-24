
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

	memset(canvas.data, 0, canvas.w * canvas.h * sizeof(pixel_t));

	canvas.incanvas_sprite_rect.x = 0;
	canvas.incanvas_sprite_rect.y = 0;
	canvas.incanvas_sprite_rect.w = 5;
	canvas.incanvas_sprite_rect.h = 10;
	canvas.incanvas_sprite_rect.origin_x = 0.5f;
	canvas.incanvas_sprite_rect.origin_y = 0.0f;
	canvas.incanvas_sprite_rect.flags.bit_set.vertical = 1;

	unsigned int w = canvas.incanvas_sprite_rect.w;
	unsigned int h = canvas.incanvas_sprite_rect.h;
	for (unsigned int y = h/2; y < h; y++)
	{
		canvas.data[w/2 + canvas.w * y] = (pixel_t){
			.r = 100,
			.g = 40,
			.b = 0,
			.a = 255
		};
	}
	for (unsigned int x = 0; x < w; x++)
	for (unsigned int y = 0; y < h/2; y++)
	{
		canvas.data[x + canvas.w * y] = (pixel_t){
			.r = 100,
			.g = 200,
			.b = 50,
			.a = 255
		};
	}

	unsigned int sprite_id_test02 = smata_register_sprite(&canvas);

	ptis_t* ptis;
	PTIS_ALLOC_SET(ptis, PTI_FLAGS, PTI_POS, PTI_SPRITEID, PTI_SCALE);
	ptis_print(ptis); printf("\n");

	oi_t oi;
	pos_t* pos;
	spriteid_t* spriteid;
	scale_t* scale;

	colt_t* colt = colt_alloc(ptis);
	colt_print(colt); printf("\n");

	oi = colt_alloc_obj(colt);
	pos = oi_get_prop(oi, PTI_POS);
	*pos = (pos_t){.x = 0.0f, .y = 0.0f, .z = 0.0f};
	spriteid = oi_get_prop(oi, PTI_SPRITEID);
	spriteid->sprite_id = sprite_id_test02;
	scale = oi_get_prop(oi, PTI_SCALE);
	scale->scale = 1.0f;
	colt_print(colt); printf("\n");

	oi = colt_alloc_obj(colt);
	pos = oi_get_prop(oi, PTI_POS);
	*pos = (pos_t){.x = 3.0f, .y = 1.0f, .z = 0.0f};
	spriteid = oi_get_prop(oi, PTI_SPRITEID);
	spriteid->sprite_id = sprite_id_test02;
	scale = oi_get_prop(oi, PTI_SCALE);
	scale->scale = 2.0f;
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
		spriteid = oi_get_prop(oi, PTI_SPRITEID);
		spriteid->sprite_id = sprite_id_test02;
		scale = oi_get_prop(oi, PTI_SCALE);
		scale->scale = 1.0f;
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
					}
				break;
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//swp_apply_on_colt(SPW_ID_POS, colt);
		swp_apply_on_colt(SPW_ID_SPRITE, colt);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
