
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

void flags_col_givetoshader(GLuint attrib_index);

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

void pos_col_givetoshader(GLuint attrib_index);

#define POS_INFO \
	{ \
		.name = "pos", \
		.size = sizeof(pos_t), \
		.col_givetoshader_callback = pos_col_givetoshader, \
	}

struct color_t
{
	float r, g, b;
};
typedef struct color_t color_t;

void color_col_givetoshader(GLuint attrib_index);

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
	PTI_COLOR,
	PROP_TYPE_COUNT
};
typedef enum pti_t pti_t;

void pti_print(pti_t pti);

struct porp_info_t
{
	const char* name;
	unsigned int size; /* sizeof(thing_t) */
	void (*col_givetoshader_callback)(GLuint attrib_index);
};
typedef struct porp_info_t porp_info_t;
extern const porp_info_t g_prop_info_table[PROP_TYPE_COUNT];

#if 0
struct ptis_t
{
	unsigned int len;
	pti_t* arr; /* Should always be sorted. */
	/* TODO: Remove the indirection, so that colt_t is faster. */
};
typedef struct ptis_t ptis_t;

void ptis_add(ptis_t* ptis, pti_t pti);
void ptis_copy(const ptis_t* src, ptis_t* dst);
int pti_eq(const ptis_t* a, const ptis_t* b);
void ptis_print(const ptis_t* ptis);
#endif

struct ptis_t
{
	unsigned int len;
	pti_t arr[]; /* Should be sorted. */
};
typedef struct ptis_t ptis_t;

#define PTIS_ALLOC(pti_ptr_, ...) \
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
void colt_print(const colt_t* colt);

#if 0
void colt_init(colt_t* colt, const ptis_t* ptis);
void colt_lengthen(colt_t* colt, unsigned int by_how_much);
int colt_does_obj_exist(const colt_t* colt, unsigned int row_index);
void colt_print(const colt_t* colt);
#endif

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

#if 0
unsigned int octa_add_colt(const ptis_t* ptis);
oi_t octa_alloc_obj(const ptis_t* ptis);
void* octa_get_obj_prop(oi_t oi, pti_t pti);
void octa_print(void);
#endif

#endif /* WHYCRYSTALS_HEADER_OCTA__ */
