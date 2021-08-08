
#ifndef WHYCRYSTALS_HEADER_VISUALS__
#define WHYCRYSTALS_HEADER_VISUALS__

struct texture_rect_t
{
	unsigned char x, y, w, h;
	float origin_x, origin_y;
};
typedef struct texture_rect_t texture_rect_t;

#define VISUAL_USED 0x1
#define VISUAL_VERTICAL 0x2

struct visual_t
{
	float x, y, z;
	float w, h;
	texture_rect_t texture_rect;
	unsigned int flags;
};
typedef struct visual_t visual_t;

void generate_texture_map(void);

#endif /* WHYCRYSTALS_HEADER_VISUALS__ */
