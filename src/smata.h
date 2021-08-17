
/* SMATA - Sprite Manager As a Texture Atlas */

#ifndef WHYCRYSTALS_HEADER_SMATA__
#define WHYCRYSTALS_HEADER_SMATA__

#include <GL/glew.h>

struct pixel_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};
typedef struct pixel_t pixel_t;

extern pixel_t* g_smata_atlas_data;
extern GLuint g_smata_atlas_id;

int init_smata(void);

#endif /* WHYCRYSTALS_HEADER_SMATA__ */
