
#include "embedded.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>

/* Wrapper around glCompileShader that prints out the error message to stderr
 * if the shader compilation fails. The dbg_info parameter can be NULL, but
 * should actually provide information that describe the shader type and shader
 * source given to glShaderSource when creating the shader (like a file path)
 * to be added in the error message if any.
 * Returns -1 if anything goes wrong, returns 0 if everything goes well. */
static int compile_shader(GLuint shader_id, const char* dbg_info)
{
	glCompileShader(shader_id);
	GLint status = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* message = malloc(length * sizeof(GLchar));
		glGetShaderInfoLog(shader_id, length, &length, message);
		fprintf(stderr,
			"OpenGL shader compilation error (%s): \x1b[31m\"%s\"\x1b[39m\n",
			(dbg_info != NULL) ? dbg_info : "\0", (const char*)message);
		return -1;
	}
	return 0;
}

/* Creates an OpenGL shader object with the given GLSL source code, then
 * compiles it and returns its id. */
static GLuint shader_src_compile(const char* shader_src, GLuint shader_type,
	const char* dbg_info)
{
	GLuint shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, (const GLchar* const*)&shader_src, NULL);
	compile_shader(shader_id, dbg_info);
	return shader_id;
}

/* Creates an OpenGL shader program object and attaches the given shaders to
 * it, then links it and returns its id.
 * The src_geom parameter can be NULL as a geometry shader is optional in the
 * OpenGL rendering pipeline. */
static GLuint shprog_build(
	const char* src_vert, const char* src_geom, const char* src_frag,
	const char* dbg_vert, const char* dbg_geom, const char* dbg_frag)
{
	GLuint shprog_id = glCreateProgram();
	GLuint shader_vert_id = shader_src_compile(src_vert,
		GL_VERTEX_SHADER, dbg_vert);
	glAttachShader(shprog_id, shader_vert_id);
	GLuint shader_geom_id;
	if (src_geom != NULL)
	{
		shader_geom_id = shader_src_compile(src_geom,
			GL_GEOMETRY_SHADER, dbg_geom);
		glAttachShader(shprog_id, shader_geom_id);
	}
	GLuint shader_frag_id = shader_src_compile(src_frag,
		GL_FRAGMENT_SHADER, dbg_frag);
	glAttachShader(shprog_id, shader_frag_id);
	glLinkProgram(shprog_id);
	glDetachShader(shprog_id, shader_vert_id);
	glDeleteShader(shader_vert_id);
	if (src_geom != NULL)
	{
		glDetachShader(shprog_id, shader_geom_id);
		glDeleteShader(shader_geom_id);
	}
	glDetachShader(shprog_id, shader_frag_id);
	glDeleteShader(shader_frag_id);
	return shprog_id;
}

/* Creates an OpenGL shader program object and attaches the given compute
 * shader to it, then links it and returns its id. */
static GLuint shprog_compute_build(const char* src_comp, const char* dbg_info)
{
	GLuint shprog_id = glCreateProgram();
	GLuint shader_comp_id = shader_src_compile(src_comp,
		GL_COMPUTE_SHADER, dbg_info);
	glAttachShader(shprog_id, shader_comp_id);
	glLinkProgram(shprog_id);
	glDetachShader(shprog_id, shader_comp_id);
	glDeleteShader(shader_comp_id);
	return shprog_id;
}

GLuint g_shprog_draw_visuals = 0;

int shprog_build_all(void)
{
	#define BUILD_SHPROG(shprog_, vert_, geom_, frag_) \
		do { \
			shprog_ = shprog_build(vert_, geom_, frag_, \
				#vert_, #geom_, #frag_); \
			if (shprog_ == 0) \
			{ \
				return -1; \
			} \
		} while (0)

	BUILD_SHPROG(g_shprog_draw_visuals,
		g_shader_visuals_vert, g_shader_visuals_geom, g_shader_visuals_frag);

	#undef BUILD_SHPROG

	return 0;
}

void shprog_destroy_all(void)
{
	glDeleteProgram(g_shprog_draw_visuals);
	g_shprog_draw_visuals = 0;
}
