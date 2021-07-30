
#include "window.h"
#include "shaders.h"
#include "random.h"
#include <stdlib.h>
#include <assert.h>

struct texture_rect_t
{
	unsigned char x, y, w, h;
};
typedef struct texture_rect_t texture_rect_t;

struct visual_t
{
	float x, y, z;
	texture_rect_t texture_rect;
};
typedef struct visual_t visual_t;

struct darray_t
{
	unsigned int len;
	unsigned int cap;
	void* array;
};
typedef struct darray_t darray_t;

#define DARRAY_INITIALIZER_EMPTY ((darray_t){0})

static unsigned int umax(unsigned int a, unsigned int b)
{
	return a > b ? a : b;
}

void darray_resize(darray_t* darray, unsigned int new_len)
{
	if (new_len > darray->cap)
	{
		unsigned int new_cap =
			umax(new_len, ((float)darray->cap + 2.3f) * 1.3f);
		void* new_array = realloc(darray->array, new_cap);
		assert(new_array != NULL);
		darray->array = new_array;
		darray->cap = new_cap;
	}
	darray->len = new_len;
}

void darray_add_len(darray_t* darray, int len_inc)
{
	darray_resize(darray, darray->len + len_inc);
}

struct tile_type_t
{
	texture_rect_t texture_rect;
};
typedef struct tile_type_t tile_type_t;

struct tile_t
{
	unsigned int visual_index;
	unsigned int type_index;
};
typedef struct tile_t tile_t;

struct object_t
{
	unsigned int visual_index;
};
typedef struct object_t object_t;

struct chunk_t
{
	int topleft_x, topleft_y;
	tile_t* tile_array;
	darray_t object_darray;
};
typedef struct chunk_t chunk_t;

struct world_t
{
	darray_t tile_type_darray;

	unsigned int chunk_side;
	darray_t chunk_darray;

	darray_t visual_darray;
};
typedef struct world_t world_t;

void world_generate(world_t* world)
{
	world->tile_type_darray = DARRAY_INITIALIZER_EMPTY;
	world->chunk_side = 9;
	world->chunk_darray = DARRAY_INITIALIZER_EMPTY;
	world->visual_darray = DARRAY_INITIALIZER_EMPTY;
}

void world_cleanup(world_t* world)
{
	
}

unsigned char* g_texture_map_data = NULL;
GLuint g_texture_map_id = 0;

void generate_g_texture_map(void)
{
	#define TEXTURE_MAP_SIDE 256
	g_texture_map_data = calloc(TEXTURE_MAP_SIDE * TEXTURE_MAP_SIDE * 4, 1);

	unsigned int visual_x, visual_y, visual_w, visual_h;

	visual_x = 0;
	visual_y = 0;
	visual_w = 8;
	visual_h = 8;
	for (unsigned int x = visual_x * visual_w; x < (visual_x+1) * visual_w; x++)
	for (unsigned int y = visual_y * visual_h; y < (visual_y+1) * visual_h; y++)
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

int main(void)
{
	if (init_g_graphics() != 0)
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

	g_rg = rg_create_timeseeded(0);

	glEnable(GL_DEPTH_TEST);

	generate_g_texture_map();

	unsigned int visual_number = 9;
	visual_t visual_array[visual_number];
	for (unsigned int i = 0; i < visual_number; i++)
	{
		visual_array[i].x = (float)(i % 3);
		visual_array[i].y = (float)(i / 3);
		visual_array[i].z = 0.0f;
		visual_array[i].texture_rect.x = 0;
		visual_array[i].texture_rect.y = 0;
		visual_array[i].texture_rect.w = 8;
		visual_array[i].texture_rect.h = 8;
	}

	GLuint buf_id_visuals;
	glGenBuffers(1, &buf_id_visuals);
	glBindBuffer(GL_ARRAY_BUFFER, buf_id_visuals);
	glBufferData(GL_ARRAY_BUFFER,
		visual_number * sizeof(visual_t),
		visual_array, GL_DYNAMIC_DRAW);

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

		glUseProgram(g_shprog_draw_visuals);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, buf_id_visuals);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(visual_t),
			(void*)offsetof(visual_t, x));
		glVertexAttribIPointer(1, 4, GL_UNSIGNED_BYTE,
			sizeof(visual_t),
			(void*)offsetof(visual_t, texture_rect));

		glDrawArrays(GL_POINTS, 0, visual_number);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram((GLuint)0);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
