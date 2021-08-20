
#include "smata.h"
#include "random.h"
#include "spw.h"
#include "utils.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>

smata_t g_smata = {0};

int init_smata(void)
{
	g_smata.atlas_data = calloc(ATLAS_SIDE * ATLAS_SIDE, sizeof(pixel_t));

	#if 0
	for (unsigned int x = 0; x < ATLAS_SIDE; x++)
	for (unsigned int y = 0; y < ATLAS_SIDE; y++)
	{
		g_smata.atlas_data[x + ATLAS_SIDE * y].r = x * 255 / ATLAS_SIDE;
		g_smata.atlas_data[x + ATLAS_SIDE * y].g = y * 255 / ATLAS_SIDE;
		g_smata.atlas_data[x + ATLAS_SIDE * y].b = 255;
		g_smata.atlas_data[x + ATLAS_SIDE * y].a = 255;
	}
	#endif

	glGenTextures(1, &g_smata.atlas_id);
	glBindTexture(GL_TEXTURE_2D, g_smata.atlas_id);
	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA, ATLAS_SIDE, ATLAS_SIDE, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, g_smata.atlas_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_smata.atlas_id);
	swp_update_atlas(0);

	glGenBuffers(1, &g_smata.sr_buffer_opengl_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_smata.sr_buffer_opengl_id);
	glBufferData(GL_ARRAY_BUFFER, g_smata.sr_len * sizeof(sprite_rect_t),
		g_smata.sr_arr, GL_DYNAMIC_DRAW);

	return 0;
}

unsigned int smata_register_sprite(canvas_t* canvas)
{
	unsigned int w = canvas->incanvas_sprite_rect.w;
	unsigned int h = canvas->incanvas_sprite_rect.h;

	unsigned int x = 0;
	unsigned int y = 0;
	while (!(
		g_smata.atlas_data[x + ATLAS_SIDE * y].r == 0 &&
		g_smata.atlas_data[x + ATLAS_SIDE * y].g == 0 &&
		g_smata.atlas_data[x + ATLAS_SIDE * y].b == 0 &&
		g_smata.atlas_data[x + ATLAS_SIDE * y].a == 0))
	{
		x++;
		if (x > ATLAS_SIDE - w)
		{
			x = 0;
			y++;
			if (y > ATLAS_SIDE - h)
			{
				assert(0);
			}
		}
		#if 0
		fprintf(stderr, "(%u, %u) -> (%u, %u, %u, %u)\n",
			x, y,
			g_smata.atlas_data[x + ATLAS_SIDE * y].r,
			g_smata.atlas_data[x + ATLAS_SIDE * y].g,
			g_smata.atlas_data[x + ATLAS_SIDE * y].b,
			g_smata.atlas_data[x + ATLAS_SIDE * y].a);
		#endif
	}

	sprite_rect_t inatlas_sprite_rect = canvas->incanvas_sprite_rect;
	inatlas_sprite_rect.x = x;
	inatlas_sprite_rect.y = y;

	for (unsigned int s_x = 0; s_x < canvas->incanvas_sprite_rect.w; s_x++)
	for (unsigned int s_y = 0; s_y < canvas->incanvas_sprite_rect.h; s_y++)
	{
		unsigned int c_x = canvas->incanvas_sprite_rect.x + s_x;
		unsigned int c_y = canvas->incanvas_sprite_rect.y + s_y;
		pixel_t pixel = canvas->data[c_x + canvas->w * c_y];
		if (pixel.a == 0)
		{
			/* Transparent pixels used in a sprite must be flagged in a way
			 * so that they are not used by an other sprite later.
			 * Unused transparent pixels are "black" and
			 * used transparent pixels are "red". */
			pixel.r = 255;
			pixel.g = 0;
			pixel.b = 0;
		}
		unsigned int a_x = inatlas_sprite_rect.x + s_x;
		unsigned int a_y = inatlas_sprite_rect.y + s_y;
		g_smata.atlas_data[a_x + ATLAS_SIDE * a_y] = pixel;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_smata.atlas_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0,
		//inatlas_sprite_rect.x, inatlas_sprite_rect.y,
		//inatlas_sprite_rect.w, inatlas_sprite_rect.h,
		0, 0, ATLAS_SIDE, ATLAS_SIDE,
		GL_RGBA, GL_UNSIGNED_BYTE, g_smata.atlas_data);
	//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	swp_update_atlas(0);

	unsigned int next_new_index = g_smata.sr_len;
	fprintf(stderr, "(len = %u) ", g_smata.sr_len);
	DA_LENGTHEN(g_smata.sr_len += 1, g_smata.sr_cap, g_smata.sr_arr,
		sprite_rect_t);
	fprintf(stderr, "to (len = %u)\n", g_smata.sr_len);
	g_smata.sr_arr[next_new_index] = inatlas_sprite_rect;

	glBindBuffer(GL_ARRAY_BUFFER, g_smata.sr_buffer_opengl_id);
	glBufferData(GL_ARRAY_BUFFER, g_smata.sr_len * sizeof(sprite_rect_t),
		g_smata.sr_arr, GL_DYNAMIC_DRAW);

	return next_new_index;
}
