
#version 430 core
layout(location = 0) uniform sampler2D u_tileset_texture;
in vec2 g_texture_pos;
out vec4 out_color;

void main()
{
	out_color = vec4(texture(u_tileset_texture, g_texture_pos));
	if (out_color.a < 0.001)
	{
		discard;
	}
}
