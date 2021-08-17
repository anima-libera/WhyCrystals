
/* Declarations for shader compilation and shader program ids.
 * Implementations are in "shaders.c". */

#ifndef WHYCRYSTALS_HEADER_SHADERS__
#define WHYCRYSTALS_HEADER_SHADERS__

#include <GL/glew.h>

/* These are OpenGL shader program ids that can be given to glUseProgram.
 * They are set to valid shader program id values by a call to shprog_build_all
 * and unset by a call to shprog_destroy_all. */
extern GLuint g_shprog_draw_visuals;
extern GLuint g_shprog_draw_pos;
extern GLuint g_shprog_draw_sprites;

/* Builds all the shader programs.
 * Returns -1 if anything goes wrong, returns 0 if everything goes well. */
int shprog_build_all(void);

/* Destroys all the shader programs built by a call to shprog_build_all. */
void shprog_destroy_all(void);

#endif /* WHYCRYSTALS_HEADER_SHADERS__ */
