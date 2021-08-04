
#include "window.h"
#include "shaders.h"
#include "random.h"
#include <stdlib.h>
#include <assert.h>

struct texture_rect_t
{
	unsigned char x, y, w, h;
	float origin_x, origin_y;
};
typedef struct texture_rect_t texture_rect_t;

#define VISUAL_VERTICAL 0x1

struct visual_t
{
	float x, y, z;
	float w, h;
	texture_rect_t texture_rect;
	unsigned int flags;
};
typedef struct visual_t visual_t;

struct darray_t
{
	unsigned int len;
	unsigned int cap;
	void* array;
};
typedef struct darray_t darray_t;

static unsigned int umax(unsigned int a, unsigned int b)
{
	return a > b ? a : b;
}

#if 0
void darray_reserve(darray_t* darray, unsigned int elem_size,
	unsigned int for_how_much)
{
	if (for_how_much > darray->cap - darray->len)
	{
		/* TODO */
	}
}
#endif

void darray_resize(darray_t* darray, unsigned int elem_size,
	unsigned int new_len)
{
	if (new_len > darray->cap)
	{
		unsigned int new_cap =
			umax(new_len, ((float)darray->cap + 2.3f) * 1.3f);
		void* new_array = realloc(darray->array, new_cap * elem_size);
		assert(new_array != NULL);
		darray->array = new_array;
		darray->cap = new_cap;
	}
	darray->len = new_len;
}

unsigned int darray_add_len(darray_t* darray, unsigned int elem_size,
	int len_inc)
{
	unsigned int old_len = darray->len;
	darray_resize(darray, elem_size, darray->len + len_inc);
	return old_len;
}

unsigned int darray_add_one(darray_t* darray, unsigned int elem_size)
{
	return darray_add_len(darray, elem_size, 1);
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
	float x, y, z;
};
typedef struct object_t object_t;

struct chunk_t
{
	int topleft_x, topleft_y;
	tile_t* tile_array;
	darray_t object_darray;
};
typedef struct chunk_t chunk_t;

struct object_index_t
{
	unsigned int chunk_index;
	unsigned int inchunk_object_index;
};
typedef struct object_index_t object_index_t;

struct world_t
{
	darray_t tile_type_darray;

	unsigned int chunk_side;
	int chunk_offset_x, chunk_offset_y;
	darray_t chunk_darray;

	object_index_t player_object_index;

	darray_t visual_darray;
	int visual_modified; /* If visual_darray data must be sent to the GPU. */
	GLuint buf_id_visuals;
};
typedef struct world_t world_t;

void world_generate(world_t* world)
{
	world->tile_type_darray = (darray_t){0};
	world->chunk_side = 9;
	world->chunk_offset_x = -4;
	world->chunk_offset_y = -4;
	world->player_object_index = (object_index_t){0};
	world->chunk_darray = (darray_t){0};
	world->visual_modified = 0;
	world->visual_darray = (darray_t){0};
}

void world_cleanup(world_t* world)
{
	
}

unsigned int generate_tile_type(world_t* world, int x, int y)
{
	if (-1 <= x && x <= 1 && -1 <= y && y <= 1)
	{
		return 1;
	}
	else
	{
		return rg_int(g_rg, 0, 1);
	}
}

void generate_tile(world_t* world, unsigned int chunk_index, int x, int y)
{
	chunk_t* chunk = &((chunk_t*)world->chunk_darray.array)[chunk_index];
	int inchunk_x = x - chunk->topleft_x;
	int inchunk_y = y - chunk->topleft_y;
	tile_t* tile =
		&chunk->tile_array[inchunk_x + world->chunk_side * inchunk_y];
	tile->type_index = generate_tile_type(world, x, y);

	unsigned int i = darray_add_one(&world->visual_darray, sizeof(visual_t));
	visual_t* visual = &((visual_t*)world->visual_darray.array)[i];
	visual->x = (float)x;
	visual->y = (float)y;
	visual->z = 0.0f;
	visual->w = 1.0f;
	visual->h = 1.0f;
	visual->texture_rect.x = 8 * tile->type_index;
	visual->texture_rect.y = 0;
	visual->texture_rect.w = 8;
	visual->texture_rect.h = 8;
	visual->texture_rect.origin_x = 0.5f;
	visual->texture_rect.origin_y = 0.5f;
	visual->flags = 0;

	tile->visual_index = i;

	world->visual_modified = 1;
}

void generate_chunk(world_t* world, unsigned int chunk_index,
	int topleft_x, int topleft_y)
{
	chunk_t* chunk = &((chunk_t*)world->chunk_darray.array)[chunk_index];
	chunk->topleft_x = topleft_x;
	chunk->topleft_y = topleft_y;
	chunk->tile_array =
		malloc(world->chunk_side * world->chunk_side * sizeof(tile_t));
	for (unsigned int inchunk_x = 0; inchunk_x < world->chunk_side; inchunk_x++)
	for (unsigned int inchunk_y = 0; inchunk_y < world->chunk_side; inchunk_y++)
	{
		generate_tile(world, chunk_index,
			chunk->topleft_x + inchunk_x, chunk->topleft_y + inchunk_y);
	}
	chunk->object_darray = (darray_t){0};
}

void generate_world_map(world_t* world)
{
	unsigned int i = darray_add_one(&world->chunk_darray, sizeof(chunk_t));
	generate_chunk(world, i, world->chunk_offset_x, world->chunk_offset_y);
}

void generate_player(world_t* world)
{
	unsigned int chunk_index = 0;
	chunk_t* chunk = &((chunk_t*)world->chunk_darray.array)[chunk_index];
	unsigned int i = darray_add_one(&chunk->object_darray, sizeof(object_t));
	object_t* object = &((object_t*)chunk->object_darray.array)[i];
	object->x = 0.0f;
	object->y = 0.0f;
	object->z = 0.0f;

	world->player_object_index.chunk_index = chunk_index;
	world->player_object_index.inchunk_object_index = i;

	unsigned int j = darray_add_one(&world->visual_darray, sizeof(visual_t));
	visual_t* visual = &((visual_t*)world->visual_darray.array)[j];
	visual->x = (float)object->x;
	visual->y = (float)object->y;
	visual->z = (float)object->z;
	visual->w = 0.15f;
	visual->h = 0.45f;
	visual->texture_rect.x = 0;
	visual->texture_rect.y = 8;
	visual->texture_rect.w = 1;
	visual->texture_rect.h = 3;
	visual->texture_rect.origin_x = 0.5f;
	visual->texture_rect.origin_y = 0.0f;
	visual->flags = 0;
	visual->flags |= VISUAL_VERTICAL;

	object->visual_index = j;

	world->visual_modified = 1;
}

void move_player(world_t* world, float diff_x, float diff_y)
{
	unsigned int chunk_index = world->player_object_index.chunk_index;
	chunk_t* chunk = &((chunk_t*)world->chunk_darray.array)[chunk_index];
	unsigned int object_index = world->player_object_index.inchunk_object_index;
	object_t* object = &((object_t*)chunk->object_darray.array)[object_index];

	object->x += diff_x;
	object->y += diff_y;

	visual_t* visual = &((visual_t*)world->visual_darray.array)[object->visual_index];

	visual->x = object->x;
	visual->y = object->y;

	world->visual_modified = 1;
}

unsigned char* g_texture_map_data = NULL;
GLuint g_texture_map_id = 0;

void generate_g_texture_map(void)
{
	#define TEXTURE_MAP_SIDE 256
	g_texture_map_data = calloc(TEXTURE_MAP_SIDE * TEXTURE_MAP_SIDE * 4, 1);

	unsigned int rect_x, rect_y, rect_w, rect_h;

	rect_x = 0;
	rect_y = 0;
	rect_w = 8;
	rect_h = 8;
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

	world_t* world = malloc(sizeof(world_t));
	world_generate(world);
	generate_world_map(world);
	generate_player(world);

	glGenBuffers(1, &world->buf_id_visuals);

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
					}
				break;
			}
		}

		if (world->visual_modified)
		{
			glBindBuffer(GL_ARRAY_BUFFER, world->buf_id_visuals);
			glBufferData(GL_ARRAY_BUFFER,
				world->visual_darray.len * sizeof(visual_t),
				world->visual_darray.array, GL_DYNAMIC_DRAW);

			world->visual_modified = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			(void*)(offsetof(visual_t, texture_rect) + offsetof(texture_rect_t, x)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			sizeof(visual_t),
			(void*)(offsetof(visual_t, texture_rect) + offsetof(texture_rect_t, origin_x)));
		glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT,
			sizeof(visual_t),
			(void*)offsetof(visual_t, flags));

		glDrawArrays(GL_POINTS, 0, world->visual_darray.len);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glUseProgram((GLuint)0);

		SDL_GL_SwapWindow(g_window);
	}

	return 0;
}
