
#include "window.h"
#include "shaders.h"
#include "random.h"
#include "octa.h"
#include "spw.h"
#include "smata.h"
#include "utils.h"
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
	/* Command line argument handling. */

	int test_random_generator = 0;
	for (unsigned int i = 1; i < (unsigned int)argc; i++)
	{
		if (strcmp(argv[i], "-r") == 0)
		{
			test_random_generator = 1;
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			fputs("bruh\n", stdout);
		}
	}

	if (test_random_generator)
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

	/* Game initialization. */

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

	/* Generating sprites. */

	canvas_t canvas;
	canvas.w = 50;
	canvas.h = 50;
	canvas.data = calloc(canvas.w * canvas.h, sizeof(pixel_t));

	#define CISR canvas.incanvas_sprite_rect

	/* Test sprite. */

	unsigned int sprite_id_test;
	{
		CISR.x = 0;
		CISR.y = 0;
		CISR.w = 8;
		CISR.h = 8;
		CISR.origin_x = 0.5f;
		CISR.origin_y = 0.5f;
		CISR.flags.bit_set.vertical = 1;

		for (unsigned int x = 0; x < CISR.w; x++)
		for (unsigned int y = 0; y < CISR.h; y++)
		{
			canvas.data[x + canvas.w * y] = (pixel_t){
				.r = rg_int(g_rg, 100, 255),
				.g = rg_int(g_rg, 0, 255),
				.b = rg_int(g_rg, 0, 100),
				.a = 255
			};
		}

		sprite_id_test = smata_register_sprite(&canvas);
	}
	(void)sprite_id_test;

	/* Player sprites. */

	unsigned int player_sprite_id_arr[2];
	{
		CISR.x = 0;
		CISR.y = 0;
		CISR.w = 5;
		CISR.h = 6;
		CISR.origin_x = 0.5f;
		CISR.origin_y = 0.0f;
		CISR.flags.bit_set.vertical = 1;

		pixel_t white = (pixel_t){
			.r = 255,
			.g = 255,
			.b = 255,
			.a = 255,
		};
		pixel_t not_white = (pixel_t){0};

		memset(canvas.data, 0, canvas.w * canvas.h * sizeof(pixel_t));

		#define PAINT(x_, y_) canvas.data[(x_) + canvas.w * (y_)] = white
		#define UNPAINT(x_, y_) canvas.data[(x_) + canvas.w * (y_)] = not_white

		/* . . @ . . *
		 * . @ @ @ . *
		 * @ . @ . @ *
		 * . . @ . . *
		 * . @ . @ . *
		 * . @ . @ . */
		PAINT(2, 0);
		PAINT(1, 1);
		PAINT(2, 1);
		PAINT(3, 1);
		PAINT(0, 2);
		PAINT(2, 2);
		PAINT(4, 2);
		PAINT(2, 3);
		PAINT(1, 4);
		PAINT(3, 4);
		PAINT(1, 5);
		PAINT(3, 5);
		player_sprite_id_arr[0] = smata_register_sprite(&canvas);

		/* . . @ . . *
		 * . @ @ @ . *
		 * @ . @ . @ *
		 * . . @ . . *
		 * . @ . @ . *
		 * @ . . . @ */
		UNPAINT(1, 5);
		UNPAINT(3, 5);
		PAINT(0, 5);
		PAINT(4, 5);
		player_sprite_id_arr[1] = smata_register_sprite(&canvas);

		#undef PAINT
		#undef UNPAINT
	}

	/* Tree sprites. */

	unsigned int tree_sprite_number = rg_int(g_rg, 1, 6);
	unsigned int tree_sprite_id_arr[tree_sprite_number];
	{
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

			CISR.x = 0;
			CISR.y = 0;
			CISR.w = w;
			CISR.h = h_bottom + h_top;
			CISR.origin_x = 0.5f;
			CISR.origin_y = 0.0f;
			CISR.flags.bit_set.vertical = 1;

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
	}

	/* Animal sprites. */

	unsigned int animal_sprite_number = rg_int(g_rg, 1, 4);
	unsigned int animal_sprite_id_arr[animal_sprite_number];
	{
		for (unsigned int i = 0; i < animal_sprite_number; i++)
		{
			memset(canvas.data, 0, canvas.w * canvas.h * sizeof(pixel_t));

			unsigned int w = 3;
			unsigned int h = 3;

			pixel_t color;
			unsigned int color_channel_sum;
			do
			{
				color = (pixel_t){
					.r = rg_int(g_rg, 0, 255),
					.g = rg_int(g_rg, 0, 255),
					.b = rg_int(g_rg, 0, 255),
					.a = 255,
				};
				color_channel_sum = color.r + color.g + color.b;
			} while (255 + 128 <= color_channel_sum && color_channel_sum <= 255 * 2 + 128);

			CISR.x = 0;
			CISR.y = 0;
			CISR.w = w;
			CISR.h = h;
			CISR.origin_x = 0.5f;
			CISR.origin_y = 0.0f;
			CISR.flags.bit_set.vertical = 1;

			#define PAINT(x_, y_) canvas.data[(x_) + canvas.w * (y_)] = color

			PAINT(0, 2);
			PAINT(0, 1);
			PAINT(0, 0);
			PAINT(1, 0);
			PAINT(2, 0);
			PAINT(2, 1);
			PAINT(2, 2);

			#undef PAINT

			animal_sprite_id_arr[i] = smata_register_sprite(&canvas);
		}
	}

	#undef CISR

	/* Player. */

	ptis_t* player_ptis;
	PTIS_ALLOC_SET(player_ptis, PTI_FLAGS, PTI_POS, PTI_SPRITEID, PTI_SCALE);
	colt_t* player_colt = colt_alloc(player_ptis);

	oi_t player_oi;
	unsigned int player_sprite_id_index = 0;
	{
		player_oi = colt_alloc_obj(player_colt);

		pos_t* pos = obj_get_prop(player_oi, PTI_POS);
		*pos = (pos_t){
			.x = 0.0f,
			.y = 0.0f,
			.z = 0.0f,
		};

		spriteid_t* spriteid = obj_get_prop(player_oi, PTI_SPRITEID);
		spriteid->sprite_id = player_sprite_id_arr[player_sprite_id_index];

		scale_t* scale = obj_get_prop(player_oi, PTI_SCALE);
		scale->scale = 1.0f;
	}

	/* Cursor. */

	ptis_t* cursor_ptis;
	PTIS_ALLOC_SET(cursor_ptis, PTI_FLAGS, PTI_POS);
	colt_t* cursor_colt = colt_alloc(cursor_ptis);

	oi_t cursor_oi;
	{
		cursor_oi = colt_alloc_obj(cursor_colt);

		pos_t* pos = obj_get_prop(cursor_oi, PTI_POS);
		*pos = (pos_t){
			.x = 0.0f,
			.y = 0.0f,
			.z = 0.0f,
		};
	}

	/* Trees. */

	ptis_t* tree_ptis;
	PTIS_ALLOC_SET(tree_ptis, PTI_FLAGS, PTI_POS, PTI_SPRITEID, PTI_SCALE);
	colt_t* tree_colt = colt_alloc(tree_ptis);

	for (unsigned int i = 0; i < 40; i++)
	{
		oi_t oi = colt_alloc_obj(tree_colt);

		pos_t* pos = obj_get_prop(oi, PTI_POS);
		*pos = (pos_t){
			.x = rg_float(g_rg, -6.5f, 6.5f),
			.y = rg_float(g_rg, -3.0f, 3.0f),
			.z = 0.0f,
		};

		spriteid_t* spriteid = obj_get_prop(oi, PTI_SPRITEID);
		spriteid->sprite_id =
			tree_sprite_id_arr[rg_int(g_rg, 0, tree_sprite_number-1)];

		scale_t* scale = obj_get_prop(oi, PTI_SCALE);
		scale->scale = 1.0f;
	}

	/* Animals. */

	ptis_t* animal_ptis;
	PTIS_ALLOC_SET(animal_ptis, PTI_FLAGS, PTI_POS, PTI_SPRITEID, PTI_SCALE);
	colt_t* animal_colt = colt_alloc(animal_ptis);

	for (unsigned int i = 0; i < 10; i++)
	{
		oi_t oi = colt_alloc_obj(animal_colt);

		pos_t* pos = obj_get_prop(oi, PTI_POS);
		*pos = (pos_t){
			.x = rg_float(g_rg, -6.5f, 6.5f),
			.y = rg_float(g_rg, -3.0f, 3.0f),
			.z = 0.0f,
		};

		spriteid_t* spriteid = obj_get_prop(oi, PTI_SPRITEID);
		spriteid->sprite_id =
			animal_sprite_id_arr[rg_int(g_rg, 0, animal_sprite_number-1)];

		scale_t* scale = obj_get_prop(oi, PTI_SCALE);
		scale->scale = 1.0f;
	}

	/* Player path. */

	float player_motion_factor = 0.05f;

	int player_is_moving = 0;
	float player_target_x, player_target_y;
	float player_motion_x, player_motion_y;
	float square_dist_to_target;

	unsigned int walking_animation_start_time;

	/* h */

	int display_positions = 0;

	float zoom_factor = 120.0f;
	//float scale_factor = 0.1f;

	unsigned int time = 0;

	int running = 1;
	while (running)
	{
		time++;
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = 0;
				break;
				case SDL_MOUSEMOTION:
					{
						pos_t* cursor_pos = obj_get_prop(cursor_oi, PTI_POS);
						cursor_pos->x = ((float)event.motion.x / (float)window_width * 2.0f - 1.0f) *
							(float)window_width / zoom_factor;
						cursor_pos->y = -((float)event.motion.y / (float)window_height * 2.0f - 1.0f) *
							(float)window_height / zoom_factor;
					}
				break;
				case SDL_MOUSEBUTTONDOWN:
					{
						player_is_moving = 1;
						const pos_t* cursor_pos = obj_get_prop(cursor_oi, PTI_POS);
						player_target_x = cursor_pos->x;
						player_target_y = cursor_pos->y;
						const pos_t* player_pos = obj_get_prop(player_oi, PTI_POS);
						float angle = atan2f(cursor_pos->y - player_pos->y, cursor_pos->x - player_pos->x);
						player_motion_x = cosf(angle) * player_motion_factor;
						player_motion_y = sinf(angle) * player_motion_factor;
						square_dist_to_target =
							squaref(player_pos->x - player_target_x) +
							squaref(player_pos->y - player_target_y);
						walking_animation_start_time = time;
					}
				break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = 0;
						break;
						case SDLK_d:
							display_positions = !display_positions;
						break;
					}
				break;
			}
		}

		if (player_is_moving)
		{
			if ((time - walking_animation_start_time) % 10 == 0)
			{
				player_sprite_id_index = 1 - player_sprite_id_index;
				spriteid_t* spriteid = obj_get_prop(player_oi, PTI_SPRITEID);
				spriteid->sprite_id = player_sprite_id_arr[player_sprite_id_index];
			}

			pos_t* player_pos = obj_get_prop(player_oi, PTI_POS);
			player_pos->x += player_motion_x;
			player_pos->y += player_motion_y;

			const float previous_square_dist_to_target = square_dist_to_target;
			square_dist_to_target =
				squaref(player_pos->x - player_target_x) +
				squaref(player_pos->y - player_target_y);
			if (square_dist_to_target >= previous_square_dist_to_target)
			{
				player_pos->x = player_target_x;
				player_pos->y = player_target_y;

				player_is_moving = 0;

				player_sprite_id_index = 0;
				spriteid_t* spriteid = obj_get_prop(player_oi, PTI_SPRITEID);
				spriteid->sprite_id = player_sprite_id_arr[player_sprite_id_index];
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (display_positions)
		{
			swp_apply_on_colt(SPW_ID_POS, tree_colt);
			swp_apply_on_colt(SPW_ID_POS, animal_colt);
			swp_apply_on_colt(SPW_ID_POS, player_colt);
		}

		swp_apply_on_colt(SPW_ID_POS, cursor_colt);

		swp_apply_on_colt(SPW_ID_SPRITE, tree_colt);
		swp_apply_on_colt(SPW_ID_SPRITE, animal_colt);
		swp_apply_on_colt(SPW_ID_SPRITE, player_colt);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
