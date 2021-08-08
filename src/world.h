
#ifndef WHYCRYSTALS_HEADER_WORLD__
#define WHYCRYSTALS_HEADER_WORLD__

#include "darray.h"
#include "visuals.h"
#include <GL/glew.h>
#include <stdint.h>

#define ENABLE_DEV_OBJECTS
#undef ENABLE_DEV_OBJECTS

enum object_type_t
{
#ifdef ENABLE_DEV_OBJECTS
	OT_DEV,
#endif
	OT_PLAYER,
	OT_ANIMAL,
	OT_SHOT,
	OT_PLANT,
	OT_UNUSED,
};
typedef enum object_type_t object_type_t;

#ifdef ENABLE_DEV_OBJECTS
struct object_dev_t
{
	float x, y, z;
	unsigned int vi;
};
typedef struct object_dev_t object_dev_t;
#endif

struct object_player_t
{
	float x, y, z;
	unsigned int body_vi;
};
typedef struct object_player_t object_player_t;

struct object_animal_t
{
	float x, y, z;
	unsigned int body_vi;
	
	float target_x, target_y;
};
typedef struct object_animal_t object_animal_t;

struct object_shot_t
{
	float x, y, z;
	unsigned int body_vi;
	
	float speed_x, speed_y, speed_z;
};
typedef struct object_shot_t object_shot_t;

struct object_plant_t
{
	float x, y, z;
	unsigned int body_vi;
};
typedef struct object_plant_t object_plant_t;

struct object_t
{
	uint32_t type;
	union
	{
		object_player_t player;
		object_animal_t animal;
		object_shot_t shot;
		object_plant_t plant;
	};
};
typedef struct object_t object_t;

struct tile_type_t
{
	texture_rect_t texture_rect;
};
typedef struct tile_type_t tile_type_t;

struct tile_t
{
	unsigned int vi;
	unsigned int type_index;
};
typedef struct tile_t tile_t;

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

object_t* object_containing(void* object_something);
void world_init(world_t* world);
void world_cleanup(world_t* world);
unsigned int generate_tile_type(world_t* world, int x, int y);
void generate_tile(world_t* world, unsigned int chunk_index, int x, int y);
void generate_chunk(world_t* world, unsigned int chunk_index,
	int topleft_x, int topleft_y);
void generate_world_map(world_t* world);
void generate_player(world_t* world);
void generate_animal(world_t* world, float x, float y);
void generate_shot(world_t* world, float x, float y);
void generate_plant(world_t* world, float x, float y);
object_player_t* get_player(world_t* world);
void generate_player_shot(world_t* world);
visual_t* get_visual(world_t* world, unsigned int vi);
void move_visual(world_t* world, unsigned int vi, float x, float y, float z);
void move_player(world_t* world, float diff_x, float diff_y);
void chunk_iter(world_t* world, unsigned int chunk_index);
void world_iter(world_t* world);

#endif /* WHYCRYSTALS_HEADER_WORLD__ */
