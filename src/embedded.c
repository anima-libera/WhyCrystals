
/* This file is overwritten at each compilation.
 * Do not modify, see "embedded.h" or "_comp.py" instead. */

/* Content of "shaders/visuals.vert". */
const char g_shader_visuals_vert[] = "\n#version 430 core\nlayout(location = 0) in vec3 in_pos;\nlayout(location = 1) in uvec4 in_texture_rect;\nout uvec4 v_texture_rect;\n\nvoid main()\n{\n\tgl_Position = vec4(in_pos, 1.0);\n\tv_texture_rect = in_texture_rect;\n}\n";

/* Content of "shaders/visuals.geom". */
const char g_shader_visuals_geom[] = "\n#version 430 core\nlayout(location = 1) uniform uvec2 u_window_wh;\nlayout(points) in;\nlayout(triangle_strip, max_vertices = 4) out;\nin uvec4 v_texture_rect[];\nout vec2 g_texture_pos;\n\n#define TAU 6.28318530717\n\nvoid main()\n{\n\tvec2 texture_rect_topleft_xy = v_texture_rect[0].xy;\n\tfloat texture_rect_w = v_texture_rect[0].z;\n\tfloat texture_rect_h = v_texture_rect[0].w;\n\tfloat texture_side = 256.0;\n\n\tvec3 pos_xyz = gl_in[0].gl_Position.xyz;\n\n\tfloat zoom_factor = 100.0;\n\n\tg_texture_pos = texture_rect_topleft_xy +\n\t\tvec2(texture_rect_w * 0.0, texture_rect_h * 0.0) / texture_side;\n\tgl_Position = vec4(pos_xyz.xy, 0, 1);\n\tgl_Position.xy += vec2(-1, -1) * 0.5;\n\tgl_Position.x *= zoom_factor / u_window_wh.x;\n\tgl_Position.y *= zoom_factor / u_window_wh.y;\n\tEmitVertex();\n\n\tg_texture_pos = texture_rect_topleft_xy +\n\t\tvec2(texture_rect_w * 1.0, texture_rect_h * 0.0) / texture_side;\n\tgl_Position = vec4(pos_xyz.xy, 0, 1);\n\tgl_Position.xy += vec2(+1, -1) * 0.5;\n\tgl_Position.x *= zoom_factor / u_window_wh.x;\n\tgl_Position.y *= zoom_factor / u_window_wh.y;\n\tEmitVertex();\n\n\tg_texture_pos = texture_rect_topleft_xy +\n\t\tvec2(texture_rect_w * 0.0, texture_rect_h * 1.0) / texture_side;\n\tgl_Position = vec4(pos_xyz.xy, 0, 1);\n\tgl_Position.xy += vec2(-1, +1) * 0.5;\n\tgl_Position.x *= zoom_factor / u_window_wh.x;\n\tgl_Position.y *= zoom_factor / u_window_wh.y;\n\tEmitVertex();\n\n\tg_texture_pos = texture_rect_topleft_xy +\n\t\tvec2(texture_rect_w * 1.0, texture_rect_h * 1.0) / texture_side;\n\tgl_Position = vec4(pos_xyz.xy, 0, 1);\n\tgl_Position.xy += vec2(+1, +1) * 0.5;\n\tgl_Position.x *= zoom_factor / u_window_wh.x;\n\tgl_Position.y *= zoom_factor / u_window_wh.y;\n\tEmitVertex();\n\n\tEndPrimitive();\n}\n";

/* Content of "shaders/visuals.frag". */
const char g_shader_visuals_frag[] = "\n#version 430 core\nlayout(location = 0) uniform sampler2D u_tileset_texture;\nin vec2 g_texture_pos;\nout vec4 out_color;\n\nvoid main()\n{\n\tout_color = vec4(texture(u_tileset_texture, g_texture_pos));\n\tif (out_color.a < 0.01)\n\t{\n\t\tdiscard;\n\t}\n}\n";
