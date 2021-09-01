
#version 430 core
layout(location = 1) uniform uvec2 u_window_wh;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
struct sprite_rect_t
{
	uint x, y, w, h;
	float origin_x, origin_y;
	uint flags;
	#define SR_FLAG_VERTICAL (1 << 0)
};
layout(shared, binding = 0) buffer sprite_rects
{
	sprite_rect_t sprite_rect_array[];
};
in uint v_flags[];
#define OBJ_FLAG_EXISTS (1 << 0)
in uint v_sprite_id[];
in float v_scale[];
out vec2 g_atlas_pos;

#define ATLAS_SIDE 1024

void main()
{
	bool exists = (v_flags[0] & OBJ_FLAG_EXISTS) != 0;

	if (!exists)
	{
		/* Discard the geometry. */
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

	sprite_rect_t sr = sprite_rect_array[v_sprite_id[0]];
	vec2 sr_xy = vec2(float(sr.x), float(sr.y)) / float(ATLAS_SIDE);
	vec2 sr_wh = vec2(float(sr.w), float(sr.h)) / float(ATLAS_SIDE);
	vec2 sr_origin = vec2(sr.origin_x, sr.origin_y);
	bool vertical = (sr.flags & SR_FLAG_VERTICAL) != 0;

	float zoom_factor = 120.0;
	float depth_factor = 0.05;
	float scale_factor = 0.1;

	vec3 pos_xyz = gl_in[0].gl_Position.xyz;
	vec2 wh = vec2(float(sr.w), float(sr.h));
	float scale = v_scale[0] * scale_factor;

	float sr_margin = 0.1 / float(ATLAS_SIDE);

	/* . . *
	 * @ . */
	g_atlas_pos = sr_xy + vec2(sr_wh.x * 0, sr_wh.y * 1);
	g_atlas_pos += vec2(+1, -1) * sr_margin;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy +=
		vec2((0.0-sr_origin.x) * wh.x, (0.0-sr_origin.y) * wh.y) * scale;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/* . . *
	 * . @ */
	g_atlas_pos = sr_xy + vec2(sr_wh.x * 1, sr_wh.y * 1);
	g_atlas_pos += vec2(-1, -1) * sr_margin;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy +=
		vec2((1.0-sr_origin.x) * wh.x, (0.0-sr_origin.y) * wh.y) * scale;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/* @ . *
	 * . . */
	g_atlas_pos = sr_xy + vec2(sr_wh.x * 0, sr_wh.y * 0);
	g_atlas_pos += vec2(+1, +1) * sr_margin;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy +=
		vec2((0.0-sr_origin.x) * wh.x, (1.0-sr_origin.y) * wh.y) * scale;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	if (vertical)
	{
		gl_Position.z -= (1.0-sr_origin.y) * wh.y * scale * depth_factor * 1.5;
	}
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/* . @ *
	 * . . */
	g_atlas_pos = sr_xy + vec2(sr_wh.x * 1, sr_wh.y * 0);
	g_atlas_pos += vec2(-1, +1) * sr_margin;
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.xy +=
		vec2((1.0-sr_origin.x) * wh.x, (1.0-sr_origin.y) * wh.y) * scale;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = gl_Position.y * depth_factor;
	if (vertical)
	{
		gl_Position.z -= (1.0-sr_origin.y) * wh.y * scale * depth_factor * 1.5;
	}
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	EndPrimitive();
}
