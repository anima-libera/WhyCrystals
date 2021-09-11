
/* OCTA - Object Container Table Array
 * Implementations are in "octa.c". */

#ifndef WHYCRYSTALS_HEADER_OCTA__
#define WHYCRYSTALS_HEADER_OCTA__

#include <GL/glew.h>

/* Octa was insipered by the concept of archetypal ECS.
 * TODO: Explain all this, and make a drawing of the memory layout. */

/* Flags property, which all objects must have (due to the design of Octa). */
struct flags_bit_set_t
{
	/* The exists flag is used by Octa at a fundamental level:
	 * A value of 0 means that the object that has it doesn't exist and the
	 * row it is in is free to be allocated for a new object.
	 * This flag should only be managed by the internals of Octa. */
	unsigned int exists: 1;
	#define OBJ_FLAG_EXISTS (1 << 0)
};
typedef struct flags_bit_set_t flags_bit_set_t;
union flags_t
{
	unsigned int plain;
	flags_bit_set_t bit_set;
};
typedef union flags_t flags_t;
void flags_col_givetoshader(GLuint* attrib_location_arr);
#define FLAGS_INFO \
	{ \
		.name = "flags", \
		.size = sizeof(flags_t), \
		.col_givetoshader_callback = flags_col_givetoshader, \
		.attrib_count = 1, \
	}

/* Position property. */
struct pos_t
{
	/* The z-axis is the down-up axis. */
	float x, y, z;
};
typedef struct pos_t pos_t;
void pos_col_givetoshader(GLuint* attrib_location_arr);
#define POS_INFO \
	{ \
		.name = "pos", \
		.size = sizeof(pos_t), \
		.col_givetoshader_callback = pos_col_givetoshader, \
		.attrib_count = 1, \
	}

/* Sprite property. */
struct sprite_t
{
	/* The id of a sprite managed by Smata, see Smata for more. */
	unsigned int sprite_id;
	/* How scaled will this sprite be displayed. */
	float scale;
};
typedef struct sprite_t sprite_t;
void sprite_col_givetoshader(GLuint* attrib_location_arr);
#define SPRITE_INFO \
	{ \
		.name = "sprite", \
		.size = sizeof(sprite_t), \
		.col_givetoshader_callback = sprite_col_givetoshader, \
		.attrib_count = 2, \
	}

/* TODO: Delete this test property. */
struct color_t
{
	float r, g, b;
};
typedef struct color_t color_t;
void color_col_givetoshader(GLuint* attrib_location_arr);
#define COLOR_INFO \
	{ \
		.name = "color", \
		.size = sizeof(color_t), \
		.col_givetoshader_callback = color_col_givetoshader, \
		.attrib_count = 1, \
	}

/* Able to walk property. */
struct walk_t
{
	int is_moving;
	float motion_factor;
	float target_x, target_y;
	float motion_x, motion_y;
	float square_dist_to_target;
};
typedef struct walk_t walk_t;
#define WALK_INFO \
	{ \
		.name = "walk", \
		.size = sizeof(walk_t), \
		.col_givetoshader_callback = NULL, \
		.attrib_count = 0, \
	}

/* Property type id. */
enum pti_t
{
	PTI_FLAGS,
	PTI_POS,
	PTI_SPRITE,
	PTI_COLOR,
	PTI_WALK,
	PROP_TYPE_COUNT
};
typedef enum pti_t pti_t;
void pti_print(pti_t pti);

/* Informations about a property type that can be accessed at runtime. */
struct porp_info_t
{
	const char* name;
	unsigned int size; /* sizeof(thing_t) */
	void (*col_givetoshader_callback)(GLuint* attrib_location_arr);
	unsigned int attrib_count; /* Shader program vertex attribute count. */
};
typedef struct porp_info_t porp_info_t;
extern const porp_info_t g_prop_info_table[PROP_TYPE_COUNT];

/* Property type id set. */
struct ptis_t
{
	unsigned int len;
	pti_t arr[]; /* Should be sorted. */
};
typedef struct ptis_t ptis_t;
#define PTIS_ALLOC(...) \
	malloc_memcpy_2( \
		sizeof(ptis_t), \
		&(ptis_t){.len = ARGS_COUNT(__VA_ARGS__)}, \
		sizeof(pti_t) * ARGS_COUNT(__VA_ARGS__), \
		ARGS_ARR_PTR(pti_t, __VA_ARGS__))
void ptis_print(const ptis_t* ptis);

/* Column. */
struct col_t
{
	pti_t pti;
	GLuint opengl_buf_id;
	int opengl_buf_needs_resize;
	void* data;
};
typedef struct col_t col_t;

/* Column table. */
struct colt_t
{
	unsigned int col_count;
	unsigned int row_count;
	col_t col_arr[];
};
typedef struct colt_t colt_t;
typedef struct oi_t oi_t;
colt_t* colt_alloc(const ptis_t* ptis);
void colt_add_rows(colt_t* colt, unsigned int how_much);
oi_t colt_alloc_obj(colt_t* colt);
col_t* colt_get_col(colt_t* colt, pti_t pti);
void colt_print(const colt_t* colt);

/* Octa, the array of tables. */
struct octa_t
{
	unsigned int len;
	colt_t** colt_arr;
};
typedef struct octa_t octa_t;
extern octa_t g_octa;

/* Object index. */
struct oi_t
{
	colt_t* colt;
	unsigned int row_index;
};
typedef struct oi_t oi_t;
void* obj_get_prop(oi_t oi, pti_t pti);

#endif /* WHYCRYSTALS_HEADER_OCTA__ */
