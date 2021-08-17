
#ifndef WHYCRYSTALS_HEADER_OCTA__
#define WHYCRYSTALS_HEADER_OCTA__

#include <GL/glew.h>

struct flags_bit_set_t
{
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
void flags_col_givetoshader(GLuint attrib_location);
#define FLAGS_INFO \
	{ \
		.name = "flags", \
		.size = sizeof(flags_t), \
		.col_givetoshader_callback = flags_col_givetoshader, \
	}

struct pos_t
{
	float x, y, z;
};
typedef struct pos_t pos_t;
void pos_col_givetoshader(GLuint attrib_location);
#define POS_INFO \
	{ \
		.name = "pos", \
		.size = sizeof(pos_t), \
		.col_givetoshader_callback = pos_col_givetoshader, \
	}

struct spriteid_t
{
	unsigned int sprite_id;
};
typedef struct spriteid_t spriteid_t;
void spriteid_col_givetoshader(GLuint attrib_location);
#define SPRITEID_INFO \
	{ \
		.name = "spriteid", \
		.size = sizeof(spriteid_t), \
		.col_givetoshader_callback = spriteid_col_givetoshader, \
	}

struct color_t
{
	float r, g, b;
};
typedef struct color_t color_t;
void color_col_givetoshader(GLuint attrib_location);
#define COLOR_INFO \
	{ \
		.name = "color", \
		.size = sizeof(color_t), \
		.col_givetoshader_callback = color_col_givetoshader, \
	}

enum pti_t
{
	PTI_FLAGS,
	PTI_POS,
	PTI_SPRITEID,
	PTI_COLOR,
	PROP_TYPE_COUNT
};
typedef enum pti_t pti_t;
void pti_print(pti_t pti);

struct porp_info_t
{
	const char* name;
	unsigned int size; /* sizeof(thing_t) */
	void (*col_givetoshader_callback)(GLuint attrib_location);
};
typedef struct porp_info_t porp_info_t;
extern const porp_info_t g_prop_info_table[PROP_TYPE_COUNT];

struct ptis_t
{
	unsigned int len;
	pti_t arr[]; /* Should be sorted. */
};
typedef struct ptis_t ptis_t;
#define PTIS_ALLOC_SET(pti_ptr_, ...) \
	do \
	{ \
		const pti_t pti_array[] = {__VA_ARGS__}; \
		pti_ptr_ = malloc(sizeof(ptis_t) + sizeof(pti_array)); \
		pti_ptr_->len = sizeof(pti_array) / sizeof(pti_t); \
		memcpy(&pti_ptr_->arr, pti_array, sizeof(pti_array)); \
	} while (0)
void ptis_print(const ptis_t* ptis);

struct col_t
{
	pti_t pti;
	GLuint opengl_buf_id;
	void* data;
};
typedef struct col_t col_t;

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
const col_t* colt_get_col(const colt_t* colt, pti_t pti);
void colt_print(const colt_t* colt);

struct octa_t
{
	unsigned int len;
	colt_t** colt_arr;
};
typedef struct octa_t octa_t;
extern octa_t g_octa;

struct oi_t
{
	colt_t* colt;
	unsigned int row_index;
};
typedef struct oi_t oi_t;
void* oi_get_prop(oi_t oi, pti_t pti);

#endif /* WHYCRYSTALS_HEADER_OCTA__ */
