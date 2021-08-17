
#version 430 core
layout(location = 0) uniform sampler2D u_atlas;
in vec2 g_atlas_pos;
out vec4 out_color;

void main()
{
	out_color = vec4(texture(u_atlas, g_atlas_pos));
	if (out_color.a < 0.01)
	{
		discard;
	}
}
