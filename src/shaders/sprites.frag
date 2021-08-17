
#version 430 core
in vec2 g_texture_pos;
out vec4 out_color;

void main()
{
	out_color = vec4(g_texture_pos, 1.0, 1.0);
}
