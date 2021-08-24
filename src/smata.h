
/* SMATA - Sprite Manager As a Texture Atlas
 * Implementations are in "smata.c". */

#ifndef WHYCRYSTALS_HEADER_SMATA__
#define WHYCRYSTALS_HEADER_SMATA__

#include <GL/glew.h>

/* Smata manages sprites in an atlas, i.e. it stores a lot of small textures
 * in a big texture, so that the rendering doesn't have to switch between
 * textures (which is costly and impractical) and instead focuces of different
 * rectangles of a single texture (which is fast and very practical). */

struct pixel_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};
typedef struct pixel_t pixel_t;

/* A field of sprite_rect_t. */
struct sprite_flags_bit_set_t
{
	/* Is the sprite vertical? A value of 0 should make the sprite horizontal
	 * in the "3D representation" of the world scene, whatever that means. */
	unsigned int vertical: 1;
};
typedef struct sprite_flags_bit_set_t sprite_flags_bit_set_t;
union sprite_flags_t
{
	unsigned int plain;
	sprite_flags_bit_set_t bit_set;
};
typedef union sprite_flags_t sprite_flags_t;

/* Rectangle indicating where is a sprite in the atlas pixel grid,
 * and additional sprite informations used during rendering and stuff. */
struct sprite_rect_t
{
	unsigned int x, y, w, h;
	float origin_x, origin_y; /* In the rect, 0 and 1 are the rect edges. */
	sprite_flags_t flags;
};
typedef struct sprite_rect_t sprite_rect_t;

/* Smata, the atlas. */
struct smata_t
{
	#define ATLAS_SIDE 1024 /* TODO: runtime get GL_MAX_TEXTURE_SIZE */
	pixel_t* atlas_data;
	GLuint atlas_id;
	unsigned int sr_len;
	unsigned int sr_cap;
	sprite_rect_t* sr_arr;
	GLuint sr_buffer_opengl_id;
};
typedef struct smata_t smata_t;
extern smata_t g_smata;
int init_smata(void);

/* Canvas are used to draw sprites one at a time and register them to smata. */
struct canvas_t
{
	unsigned int w, h;
	pixel_t* data;
	sprite_rect_t incanvas_sprite_rect;
};
typedef struct canvas_t canvas_t;
unsigned int smata_register_sprite(canvas_t* canvas);

#endif /* WHYCRYSTALS_HEADER_SMATA__ */
