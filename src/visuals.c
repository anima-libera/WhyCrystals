
#include "visuals.h"
#include "random.h"
#include "shaders.h"
#include <stdlib.h>
#include <GL/glew.h>

unsigned char* g_texture_map_data = NULL;
GLuint g_texture_map_id = 0;

void generate_texture_map(void)
{
	#define TEXTURE_MAP_SIDE 256
	g_texture_map_data = calloc(TEXTURE_MAP_SIDE * TEXTURE_MAP_SIDE * 4, 1);

	unsigned int rect_x, rect_y, rect_w, rect_h;
	float rect_origin_x, rect_origin_y;

	rect_x = 0;
	rect_y = 0;
	rect_w = 8;
	rect_h = 8;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.5f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		unsigned char r = rg_int(g_rg, 0, 10);
		unsigned char g = rg_int(g_rg, 0, 140);
		unsigned char b = 255 - rg_int(g_rg, 0, 90);
		unsigned char a = 255;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	rect_x = 8;
	rect_y = 0;
	rect_w = 8;
	rect_h = 8;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.5f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		unsigned char r = rg_int(g_rg, 0, 60);
		unsigned char g = 255 - rg_int(g_rg, 0, 60);
		unsigned char b = rg_int(g_rg, 0, 60);
		unsigned char a = 255;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	rect_x = 0;
	rect_y = 8;
	rect_w = 1;
	rect_h = 3;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.0f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		unsigned char r = 255;
		unsigned char g = y * 20;
		unsigned char b = 255;
		unsigned char a = 255;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	rect_x = 1;
	rect_y = 8;
	rect_w = 3;
	rect_h = 3;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.0f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		int ix = x - rect_x, iy = y - rect_y;
		int c = !(ix == 1 && iy >= 1);
		unsigned char r = 255;
		unsigned char g = 100;
		unsigned char b = 0;
		unsigned char a = 255 * c;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	rect_x = 4;
	rect_y = 8;
	rect_w = 1;
	rect_h = 1;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.5f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		unsigned char r = 255;
		unsigned char g = 255;
		unsigned char b = 40;
		unsigned char a = 255;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	rect_x = 5;
	rect_y = 8;
	rect_w = 5;
	rect_h = 3;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.5f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		int ix = x - rect_x, iy = y - rect_y;
		int c = !((ix == 0 || ix == 4) && (iy == 0 || iy == 2));
		unsigned char r = 255;
		unsigned char g = 100;
		unsigned char b = 0;
		unsigned char a = 255 * c;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	rect_x = 10;
	rect_y = 8;
	rect_w = 3;
	rect_h = 7;
	rect_origin_x = 0.5f;
	rect_origin_y = 0.0f;
	for (unsigned int x = rect_x; x < rect_x + rect_w; x++)
	for (unsigned int y = rect_y; y < rect_y + rect_h; y++)
	{
		int ix = x - rect_x, iy = y - rect_y;
		unsigned char r = 200 * (iy > 2);
		unsigned char g = 100 * (iy > 2) + 130 * (iy <= 2);
		unsigned char b = 0;
		unsigned char a = 255 * (iy <= 2 || ix == 1);
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 0] = r;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 1] = g;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 2] = b;
		g_texture_map_data[(x + y * TEXTURE_MAP_SIDE) * 4 + 3] = a;
	}

	(void)rect_origin_x;
	(void)rect_origin_y;

	glGenTextures(1, &g_texture_map_id);
	glBindTexture(GL_TEXTURE_2D, g_texture_map_id);
	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA, TEXTURE_MAP_SIDE, TEXTURE_MAP_SIDE, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, g_texture_map_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture_map_id);
	glProgramUniform1i(g_shprog_draw_visuals, 0, 0);
}
