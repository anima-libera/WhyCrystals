
#version 430 core
layout(location = 0) in vec3 in_pos;
layout(location = 1) in uvec4 in_texture_rect;
out uvec4 v_texture_rect;

void main()
{
	gl_Position = vec4(in_pos, 1.0);
	v_texture_rect = in_texture_rect;
}
