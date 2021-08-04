
#version 430 core
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_wh;
layout(location = 2) in uvec4 in_texture_rect;
layout(location = 3) in vec2 in_texture_rect_origin;
layout(location = 4) in uint in_flags;
out vec2 v_wh;
out uvec4 v_texture_rect;
out vec2 v_texture_rect_origin;
out uint v_flags;

void main()
{
	gl_Position = vec4(in_pos, 1.0);
	v_wh = in_wh;
	v_texture_rect = in_texture_rect;
	v_texture_rect_origin = in_texture_rect_origin;
	v_flags = in_flags;
}
