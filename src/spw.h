
/* Shader program wrappers for an easy handling of shaders.
 * Implementations are in "spw.c". */

#ifndef WHYCRYSTALS_HEADER_SPW__
#define WHYCRYSTALS_HEADER_SPW__

#include "octa.h"
#include <GL/glew.h>

enum spw_uniform_whichone_t
{
	U_ATLAS,
	U_WINDOW_WH,
	UNIFORM_WHICHONE_COUNT
};
typedef enum spw_uniform_whichone_t spw_uniform_whichone_t;

struct spw_uniform_t
{
	spw_uniform_whichone_t whichone;
	GLuint location;
};
typedef struct spw_uniform_t spw_uniform_t;

enum spw_buffer_whichone_t
{
	B_SMATA_SPRITE_RECTS,
	BUFFER_WHICHONE_COUNT
};
typedef enum spw_buffer_whichone_t spw_buffer_whichone_t;

struct spw_buffer_t
{
	spw_buffer_whichone_t whichone;
	GLuint binding;
};
typedef struct spw_buffer_t spw_buffer_t;

struct spw_attrib_t
{
	pti_t pti;
	GLuint location; /* Or index ??? */
};
typedef struct spw_attrib_t spw_attrib_t;

/* Shader program wrapper. */
struct spw_t
{
	GLuint shprog_id;
	unsigned int uniform_count;
	spw_uniform_t* uniform_arr;
	unsigned int buffer_count;
	spw_buffer_t* buffer_arr;
	unsigned int attrib_count;
	spw_attrib_t* attrib_arr;
};
typedef struct spw_t spw_t;

enum spw_id_t
{
	SPW_ID_POS,
	SPW_ID_SPRITE,
	SPW_COUNT
};
typedef enum spw_id_t spw_id_t;
spw_t g_spw_table[SPW_COUNT];
int init_swp_table(void);
void swp_apply_on_colt(spw_id_t spw_id, colt_t* colt);

void swp_update_atlas(unsigned int texture_unit_id);
void swp_update_window_wh(unsigned int w, unsigned int h);

#endif /* WHYCRYSTALS_HEADER_SPW__ */
