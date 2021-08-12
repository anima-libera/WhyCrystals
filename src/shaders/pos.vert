
#version 430 core
layout(location = 0) in uint in_flags;
layout(location = 1) in vec3 in_pos;
out uint v_flags;

void main()
{
	gl_Position = vec4(in_pos, 1.0);
	v_flags = in_flags;
}
