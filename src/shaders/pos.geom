
#version 430 core
layout(location = 1) uniform uvec2 u_window_wh;
layout(points) in;
layout(line_strip, max_vertices = 5) out;
in uint v_flags[];

#define OBJ_FLAG_EXISTS (1 << 0)

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
		gl_Position = vec4(-2, -2, -2, 1);
		EmitVertex();
		EndPrimitive();
		return;
	}

	vec3 pos_xyz = gl_in[0].gl_Position.xyz;
	float zoom_factor = 120.0;
	float depth_factor = 0.05;

	/*   @   *
	 * . . . *
	 *   .   */
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += 0.0;
	gl_Position.y += +0.5;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = -1.0;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/*   .   *
	 * . . . *
	 *   @   */
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += 0.0;
	gl_Position.y += -0.5;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = -1.0;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/*   .   *
	 * . @ . *
	 *   .   */
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += 0.0;
	gl_Position.y += 0.0;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = -1.0;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/*   .   *
	 * @ . . *
	 *   .   */
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += -0.5;
	gl_Position.y += 0.0;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = -1.0;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	/*   .   *
	 * . . @ *
	 *   .   */
	gl_Position = vec4(pos_xyz.xy, 0, 1);
	gl_Position.x += +0.5;
	gl_Position.y += 0.0;
	gl_Position.y += pos_xyz.z;
	gl_Position.z = -1.0;
	gl_Position.x *= zoom_factor / u_window_wh.x;
	gl_Position.y *= zoom_factor / u_window_wh.y;
	EmitVertex();

	EndPrimitive();
}
