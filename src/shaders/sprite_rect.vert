
#version 430 core
layout(location = 0) in uint in_flags;
layout(location = 1) in vec3 in_pos;
layout(location = 2) in uint in_sprite_id;
layout(location = 3) in float in_scale;
out uint v_flags;
out uint v_sprite_id;
out float v_scale;

void main()
{
	gl_Position = vec4(in_pos, 1.0);
	v_flags = in_flags;
	v_sprite_id = in_sprite_id;
	v_scale = in_scale;
}
