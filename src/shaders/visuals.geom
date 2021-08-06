
#version 430 core
layout(location = 1) uniform uvec2 u_window_wh;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
in vec2 v_wh[];
in uvec4 v_texture_rect[];
in vec2 v_texture_rect_origin[];
in uint v_flags[];
out vec2 g_texture_pos;

#define VISUAL_USED 0x1
#define VISUAL_VERTICAL 0x2

void main()
{
	vec2 texture_rect_topleft_xy = v_texture_rect[0].xy;
	float texture_rect_w = v_texture_rect[0].z;
	float texture_rect_h = v_texture_rect[0].w;
	vec2 texture_rect_origin_xy = v_texture_rect_origin[0];
	float texture_side = 256.0;

	vec3 pos_xyz = gl_in[0].gl_Position.xyz;
	float w = v_wh[0].x;
	float h = v_wh[0].y;

	float left_offset = -texture_rect_origin_xy.x * w;
	float right_offset = (1.0 - texture_rect_origin_xy.x) * w;
	float top_offset = (1.0 - texture_rect_origin_xy.y) * h;
	float bottom_offset = -texture_rect_origin_xy.y * h;

	bool used = (v_flags[0] & VISUAL_USED) != 0;
	bool vertical = (v_flags[0] & VISUAL_VERTICAL) != 0;

	float zoom_factor = 120.0;
	float depth_factor = 0.05;

	if (!used)
	{
		gl_Position = vec4(-2, -2, -2, 1);
		EmitVertex();
		gl_Position = vec4(-2, -2, -2, 1);
		EmitVertex();
		gl_Position = vec4(-2, -2, -2, 1);
		EmitVertex();
		gl_Position = vec4(-2, -2, -2, 1);
		EmitVertex();
		EndPrimitive();
		return;
	}

	g_texture_pos = texture_rect_topleft_xy;
	g_texture_pos += vec2(texture_rect_w * 0.0, texture_rect_h * 0.0);
	g_texture_pos /= texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += left_offset;
	gl_Position.y += top_offset;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	if (vertical)
	{
		gl_Position.z -= h * depth_factor * 1.5;
	}
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	g_texture_pos = texture_rect_topleft_xy;
	g_texture_pos += vec2(texture_rect_w * 1.0, texture_rect_h * 0.0);
	g_texture_pos /= texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += right_offset;
	gl_Position.y += top_offset;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	if (vertical)
	{
		gl_Position.z -= h * depth_factor * 1.5;
	}
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	g_texture_pos = texture_rect_topleft_xy;
	g_texture_pos += vec2(texture_rect_w * 0.0, texture_rect_h * 1.0);
	g_texture_pos /= texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += left_offset;
	gl_Position.y += bottom_offset;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	g_texture_pos = texture_rect_topleft_xy;
	g_texture_pos += vec2(texture_rect_w * 1.0, texture_rect_h * 1.0);
	g_texture_pos /= texture_side;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += right_offset;
	gl_Position.y += bottom_offset;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	EndPrimitive();
}
