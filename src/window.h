
/* Related to the SDL2 window and the OpenGL context.
 * Implementations are in "window.c". */

#ifndef CTC_HEADER_WINDOW__
#define CTC_HEADER_WINDOW__

#include <SDL2/SDL.h>

/* The unique global window.
 * It is created by init_g_graphics and destroyed by cleanup_g_graphics. */
extern SDL_Window* g_window;

/* Initializes the SDL2 library and the GLEW OpenGL extension loader.
 * Creates the unique global window g_window and the unique global OpenGL
 * context g_opengl_context.
 * Returns -1 if anything goes wrong, returns 0 if everything goes well.
 * This function should be called once at the beginning of execution, and
 * a unique maching call to cleanup_g_graphics should be done at the end of
 * execution. */
int init_g_graphics(void);

/* Cleans up and destroys what is initialized and created
 * by init_g_graphics. */
void cleanup_g_graphics(void);

#endif /* CTC_HEADER_WINDOW__ */
