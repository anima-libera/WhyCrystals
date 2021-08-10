
#include "world.h"
#include "visuals.h"
#include "random.h"
#include "da.h"
#include <GL/glew.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define TAU 6.28318530717

obj_t* obj_containing(void* obj_something)
{
	return (obj_t*)(((char*)obj_something) - 4);
}

unsigned int chunk_allocate_obj(chunk_t* chunk)
{
	for (unsigned int i = 0; i < chunk->obj_da.len; i++)
	{
		obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), i);
		int is_unused = obj->type == OT_UNUSED;
		da_giveback_ptr(&chunk->obj_da, sizeof(obj_t), obj);
		if (is_unused)
		{
			return i;
		}
	}
	return da_add_one(&chunk->obj_da, sizeof(obj_t));
}

/* TODO !!!!!!!!!! ! ! ! !!!!!!!!!!!!!!!! ! ! ! !!!!!!!!!!!!!!!!! ! ! !!!!!!!!!!!!!!!!
 * use da_take_ptr and da_giveback_ptr everywhere here,
 * taking notes about how to make it easier and/or less verbose,
 * then implement these solutions and repeat, until satisfying
 * !!!!!!!!!! ! ! ! !!!!!!!!!!!!!!!! ! ! ! ! !!!!!!!!!!!!!!!!!!!!!!!!!! ! ! ! !!!!!!!!!!!!! */

#if 0
obj_t* chunk_get_obj(chunk_t* chunk, unsigned int obj_index)
{
	return &((obj_t*)chunk->obj_da.arr)[obj_index];
}
#endif

void world_init(world_t* world)
{
	world->tile_type_da = (da_t){0};
	world->chunk_side = 9;
	world->chunk_offset_x = -4;
	world->chunk_offset_y = -4;
	world->chunk_da = (da_t){0};
	world->player_obj_index = (obj_index_t){0};
	world->visual_da = (da_t){0};
	world->visual_modified = 0;
	world->buf_id_visuals = 0;
}

void world_cleanup(world_t* world)
{
	(void)world;
}

chunk_t* world_get_chunk(world_t* world, unsigned int chunk_index)
{
	return &((chunk_t*)world->chunk_da.arr)[chunk_index];
}

unsigned int generate_tile_type(world_t* world, int x, int y)
{
	(void)world;
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
	chunk_t* chunk = &((chunk_t*)world->chunk_da.arr)[chunk_index];
	int inchunk_x = x - chunk->topleft_x;
	int inchunk_y = y - chunk->topleft_y;
	unsigned int tile_index = inchunk_x + world->chunk_side * inchunk_y;
	tile_t* tile = &chunk->tile_arr[tile_index];
	tile->type_index = generate_tile_type(world, x, y);

	unsigned int vi = da_add_one(&world->visual_da, sizeof(visual_t));
	visual_t* visual = &((visual_t*)world->visual_da.arr)[vi];
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
	visual->flags = VISUAL_USED;

	tile->vi = vi;

	world->visual_modified = 1;
}

void generate_chunk(world_t* world, unsigned int chunk_index,
	int topleft_x, int topleft_y)
{
	chunk_t* chunk = &((chunk_t*)world->chunk_da.arr)[chunk_index];
	chunk->topleft_x = topleft_x;
	chunk->topleft_y = topleft_y;
	chunk->tile_arr =
		malloc(world->chunk_side * world->chunk_side * sizeof(tile_t));
	chunk->obj_da = (da_t){0};
	for (unsigned int inchunk_x = 0; inchunk_x < world->chunk_side; inchunk_x++)
	for (unsigned int inchunk_y = 0; inchunk_y < world->chunk_side; inchunk_y++)
	{
		generate_tile(world, chunk_index,
			chunk->topleft_x + inchunk_x, chunk->topleft_y + inchunk_y);
	}
}

void generate_world_map(world_t* world)
{
	unsigned int chunk_index = da_add_one(&world->chunk_da, sizeof(chunk_t));
	generate_chunk(world, chunk_index,
		world->chunk_offset_x, world->chunk_offset_y);
}

visual_t* get_visual(world_t* world, unsigned int vi)
{
	return &((visual_t*)world->visual_da.arr)[vi];
}

unsigned int allocate_visual(world_t* world)
{
	for (unsigned int i = 0; i < world->visual_da.len; i++)
	{
		visual_t* visual = get_visual(world, i);
		if ((visual->flags & VISUAL_USED) == 0)
		{
			return i;
		}
	}
	return da_add_one(&world->visual_da, sizeof(visual_t));
}

void unuse_visual(world_t* world, unsigned int vi)
{
	visual_t* visual = get_visual(world, vi);
	if (visual->flags & VISUAL_USED)
	{
		visual->flags -= VISUAL_USED;
	}
	world->visual_modified = 1;
}

void generate_player(world_t* world)
{
	unsigned int chunk_index = 0;
	chunk_t* chunk = world_get_chunk(world, chunk_index);
	unsigned int inchunk_obj_index = chunk_allocate_obj(chunk);
	obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), inchunk_obj_index);

	obj->type = OT_PLAYER;
	obj_player_t* player = &obj->player;

	player->x = 0.0f;
	player->y = 0.0f;
	player->z = 0.0f;

	world->player_obj_index.chunk_index = chunk_index;
	world->player_obj_index.inchunk_obj_index = inchunk_obj_index;

	player->body_vi = allocate_visual(world);
	visual_t* visual = get_visual(world, player->body_vi);
	visual->x = (float)player->x;
	visual->y = (float)player->y;
	visual->z = (float)player->z;
	visual->w = 0.15f;
	visual->h = 0.45f;
	visual->texture_rect.x = 0;
	visual->texture_rect.y = 8;
	visual->texture_rect.w = 1;
	visual->texture_rect.h = 3;
	visual->texture_rect.origin_x = 0.5f;
	visual->texture_rect.origin_y = 0.0f;
	visual->flags = VISUAL_USED | VISUAL_VERTICAL;

	world->visual_modified = 1;

	da_giveback_ptr(&chunk->obj_da, sizeof(obj_t), obj);
}

void generate_animal(world_t* world, float x, float y)
{
	unsigned int chunk_index = 0;
	chunk_t* chunk = world_get_chunk(world, chunk_index);
	unsigned int inchunk_obj_index = chunk_allocate_obj(chunk);
	obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), inchunk_obj_index);

	obj->type = OT_ANIMAL;
	obj_animal_t* animal = &obj->animal;

	animal->x = x;
	animal->y = y;
	animal->z = 0.0f;
	animal->target_x = animal->x;
	animal->target_y = animal->y;

	animal->body_vi = allocate_visual(world);
	visual_t* visual = get_visual(world, animal->body_vi);
	visual->x = (float)animal->x;
	visual->y = (float)animal->y;
	visual->z = (float)animal->z;
	visual->w = 0.4f;
	visual->h = 0.4f;
	visual->texture_rect.x = 1;
	visual->texture_rect.y = 8;
	visual->texture_rect.w = 3;
	visual->texture_rect.h = 3;
	visual->texture_rect.origin_x = 0.5f;
	visual->texture_rect.origin_y = 0.0f;
	visual->flags = VISUAL_USED | VISUAL_VERTICAL;

	world->visual_modified = 1;

	da_giveback_ptr(&chunk->obj_da, sizeof(obj_t), obj);
}

void generate_shot(world_t* world, float x, float y)
{
	fprintf(stderr, "A\n");
	unsigned int chunk_index = 0;
	chunk_t* chunk = world_get_chunk(world, chunk_index);
	unsigned int inchunk_obj_index = chunk_allocate_obj(chunk);
	obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), inchunk_obj_index);
	fprintf(stderr, "B\n");

	obj->type = OT_SHOT;
	obj_shot_t* shot = &obj->shot;

	shot->x = x;
	shot->y = y;
	shot->z = 0.3f;
	shot->speed_x = 0.6f;
	shot->speed_y = 0.0f;
	shot->speed_z = 0.0f;

	shot->body_vi = allocate_visual(world);
	visual_t* visual = get_visual(world, shot->body_vi);
	visual->x = (float)shot->x;
	visual->y = (float)shot->y;
	visual->z = (float)shot->z;
	visual->w = 0.1f;
	visual->h = 0.1f;
	visual->texture_rect.x = 4;
	visual->texture_rect.y = 8;
	visual->texture_rect.w = 1;
	visual->texture_rect.h = 1;
	visual->texture_rect.origin_x = 0.5f;
	visual->texture_rect.origin_y = 0.5f;
	visual->flags = VISUAL_USED | VISUAL_VERTICAL;

	world->visual_modified = 1;

	da_giveback_ptr(&chunk->obj_da, sizeof(obj_t), obj);
}

void generate_plant(world_t* world, float x, float y)
{
	unsigned int chunk_index = 0;
	chunk_t* chunk = &((chunk_t*)world->chunk_da.arr)[chunk_index];
	unsigned int inchunk_obj_index = chunk_allocate_obj(chunk);
	obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), inchunk_obj_index);

	obj->type = OT_PLANT;
	obj_plant_t* plant = &obj->plant;

	plant->x = x;
	plant->y = y;
	plant->z = 0.0f;

	plant->body_vi = allocate_visual(world);
	visual_t* visual = get_visual(world, plant->body_vi);
	visual->x = (float)plant->x;
	visual->y = (float)plant->y;
	visual->z = (float)plant->z;
	visual->w = 0.3f * 0.8f;
	visual->h = 0.7f * 0.8f;
	visual->texture_rect.x = 10;
	visual->texture_rect.y = 8;
	visual->texture_rect.w = 3;
	visual->texture_rect.h = 7;
	visual->texture_rect.origin_x = 0.5f;
	visual->texture_rect.origin_y = 0.0f;
	visual->flags = VISUAL_USED | VISUAL_VERTICAL;

	world->visual_modified = 1;

	da_giveback_ptr(&chunk->obj_da, sizeof(obj_t), obj);
}

obj_player_t* take_player(world_t* world)
{
	unsigned int chunk_index = world->player_obj_index.chunk_index;
	chunk_t* chunk = world_get_chunk(world, chunk_index);
	unsigned int inchunk_obj_index = world->player_obj_index.inchunk_obj_index;
	obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), inchunk_obj_index);

	assert(obj->type == OT_PLAYER);
	obj_player_t* player = &obj->player;

	return player;
}

void giveback_player(world_t* world, obj_player_t* player)
{
	obj_t* obj = obj_containing(player);
	assert(obj->type == OT_PLAYER);

	unsigned int chunk_index = world->player_obj_index.chunk_index;
	chunk_t* chunk = world_get_chunk(world, chunk_index);

	da_giveback_ptr(&chunk->obj_da, sizeof(obj_t), obj);
}

void get_player_coords(world_t* world,
	float* out_x, float* out_y, float* out_z)
{
	obj_player_t* player = take_player(world);
	if (out_x != NULL)
	{
		*out_x = player->x;
	}
	if (out_y != NULL)
	{
		*out_y = player->y;
	}
	if (out_z != NULL)
	{
		*out_z = player->z;
	}
	giveback_player(world, player);
}

void generate_player_shot(world_t* world)
{
	float x, y;
	get_player_coords(world, &x, &y, NULL);
	generate_shot(world, x, y);
}

void move_visual(world_t* world, unsigned int vi, float x, float y, float z)
{
	visual_t* visual = get_visual(world, vi);

	visual->x = x;
	visual->y = y;
	visual->z = z;

	world->visual_modified = 1;
}

void move_player(world_t* world, float diff_x, float diff_y)
{
	obj_player_t* player = take_player(world);

	player->x += diff_x;
	player->y += diff_y;
	move_visual(world, player->body_vi, player->x, player->y, 0.0f);

	giveback_player(world, player);
}

float length(float x, float y, float z)
{
	return sqrtf(x * x + y * y + z * z);
}

float dist(float x_a, float y_a, float z_a, float x_b, float y_b, float z_b)
{
	return length(x_a - x_b, y_a - y_b, z_a - z_b);
}

void animal_iter(world_t* world, unsigned int chunk_index,
	obj_animal_t* animal)
{
	(void)chunk_index;

	float move_x = animal->target_x - animal->x;
	float move_y = animal->target_y - animal->y;

	float move_speed = 0.02f;
	float move_dist = length(move_x, move_y, 0.0f);

	if (move_dist < move_speed)
	{
		animal->x = animal->target_x;
		animal->y = animal->target_y;
		move_visual(world, animal->body_vi, animal->x, animal->y, 0.0f);

		if (rg_int(g_rg, 0, 500) == 0)
		{
			float angle = rg_float(g_rg, 0.0f, TAU);
			float dist = rg_float(g_rg, 0.4f, 3.0f);

			animal->target_x = animal->x + cosf(angle) * dist;
			animal->target_y = animal->y + sinf(angle) * dist;
		}
	}
	else
	{
		float norm_move_x = move_x / move_dist;
		float norm_move_y = move_y / move_dist;

		float speed_x = norm_move_x * move_speed;
		float speed_y = norm_move_y * move_speed;

		animal->x += speed_x;
		animal->y += speed_y;
		move_visual(world, animal->body_vi, animal->x, animal->y, 0.0f);
	}
}

void shot_iter(world_t* world, unsigned int chunk_index,
	obj_shot_t* shot)
{

	shot->x += shot->speed_x;
	shot->y += shot->speed_y;
	shot->z += shot->speed_z;
	move_visual(world, shot->body_vi, shot->x, shot->y, shot->z);

	shot->speed_z -= 0.01f;

	shot->speed_x *= 0.98f;
	shot->speed_y *= 0.98f;
	shot->speed_z *= 0.98f;

	if (shot->z <= 0.0f)
	{
		obj_t* obj = obj_containing(shot);
		obj->type = OT_UNUSED;

		unuse_visual(world, shot->body_vi);
	}

	chunk_t* chunk = world_get_chunk(world, chunk_index);
	for (unsigned int i = 0; i < chunk->obj_da.len; i++)
	{
		obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), i);
		if (obj->type == OT_ANIMAL)
		{
			obj_animal_t* animal = &obj->animal;
			float d = dist(shot->x, shot->y, shot->z,
				animal->x, animal->y, animal->z);
			if (d < 0.5f)
			{
				obj_t* obj = obj_containing(animal);
				obj->type = OT_UNUSED;

				unuse_visual(world, animal->body_vi);

				obj = obj_containing(shot);
				obj->type = OT_UNUSED;

				unuse_visual(world, shot->body_vi);
			}
		}
	}
}

void chunk_iter(world_t* world, unsigned int chunk_index)
{
	chunk_t* chunk = world_get_chunk(world, chunk_index);
	for (unsigned int i = 0; i < chunk->obj_da.len; i++)
	{
		obj_t* obj = da_take_ptr(&chunk->obj_da, sizeof(obj_t), i);
		if (obj->type == OT_ANIMAL)
		{
			obj_animal_t* animal = &obj->animal;
			animal_iter(world, chunk_index, animal);
		}
		else if (obj->type == OT_SHOT)
		{
			obj_shot_t* shot = &obj->shot;
			shot_iter(world, chunk_index, shot);
		}
	}
}

void world_iter(world_t* world)
{
	for (unsigned int i = 0; i < world->chunk_da.len; i++)
	{
		chunk_iter(world, i);
	}
}
