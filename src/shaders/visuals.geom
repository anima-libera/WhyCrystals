
#version 430 core
layout(location = 1) uniform uvec2 u_window_wh;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
in uvec4 v_texture_rect[];
out vec2 g_texture_pos;

#define TAU 6.28318530717

void main()
{
	vec2 texture_rect_topleft_xy = v_texture_rect[0].xy;
	float texture_rect_w = v_texture_rect[0].z;
	float texture_rect_h = v_texture_rect[0].w;
	float texture_side = 256.0;

	vec3 pos_xyz = gl_in[0].gl_Position.xyz;

	float zoom_factor = 100.0;

	g_texture_pos = texture_rect_topleft_xy +
		vec2(texture_rect_w * 0.0, texture_rect_h * 0.0) / texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy += vec2(-1, -1) * 0.5;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	g_texture_pos = texture_rect_topleft_xy +
		vec2(texture_rect_w * 1.0, texture_rect_h * 0.0) / texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy += vec2(+1, -1) * 0.5;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	g_texture_pos = texture_rect_topleft_xy +
		vec2(texture_rect_w * 0.0, texture_rect_h * 1.0) / texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy += vec2(-1, +1) * 0.5;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	g_texture_pos = texture_rect_topleft_xy +
		vec2(texture_rect_w * 1.0, texture_rect_h * 1.0) / texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy += vec2(+1, +1) * 0.5;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	EndPrimitive();
}
