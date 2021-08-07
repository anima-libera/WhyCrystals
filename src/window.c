
#include "window.h"
#include "dbgmsg.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

SDL_Window* g_window = NULL;

/* The unique global SDL2-window-compatible OpenGL context.
 * It is created by init_g_graphics and destroyed by cleanup_g_graphics. */
static SDL_GLContext s_opengl_context = NULL;

/* The OpenGL VAO generated and bound by init_g_graphics a */
static GLuint s_vao_id = 0;

int init_g_graphics(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		error_sdl2_fail("SDL_Init");
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	g_window = SDL_CreateWindow("Why Crystals",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (g_window == NULL)
	{
		error_sdl2_fail("SDL_CreateWindow");
		return -1;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0)
	{
		error_sdl2_fail(
			"SDL_GL_SetAttribute with SDL_GL_CONTEXT_MAJOR_VERSION");
		return -1;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0)
	{
		error_sdl2_fail(
			"SDL_GL_SetAttribute with SDL_GL_CONTEXT_MINOR_VERSION");
		return -1;
	}
	if (SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
	{
		error_sdl2_fail(
			"SDL_GL_SetAttribute with SDL_GL_CONTEXT_PROFILE_MASK");
		return -1;
	}
	s_opengl_context = SDL_GL_CreateContext(g_window);
	if (s_opengl_context == NULL)
	{
		error_sdl2_fail("SDL_GL_CreateContext");
		return -1;
	}
	GLenum gnew_init_result = glewInit();
	if (gnew_init_result != GLEW_OK)
	{
		fprintf(stderr, "GLEW error: glewInit failed: \"%s\"\n",
			glewGetErrorString(gnew_init_result));
	}
	glEnable(GL_MULTISAMPLE);
	if (SDL_GL_SetSwapInterval(-1) != 0)
	{
		SDL_GL_SetSwapInterval(1);
	}
	enable_opengl_dbgmsg();
	glGenVertexArrays(1, &s_vao_id);
	glBindVertexArray(s_vao_id);
	return 0;
}

void cleanup_g_graphics(void)
{
	glDeleteVertexArrays(1, &s_vao_id);
	s_vao_id = 0;
	SDL_GL_DeleteContext(s_opengl_context);
	s_opengl_context = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	SDL_Quit();
}
