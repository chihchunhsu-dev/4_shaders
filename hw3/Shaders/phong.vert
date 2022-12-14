#version 410

layout(location = 0) in vec4 in_pos;
layout(location = 1) in vec4 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec2 io_tex_coord;
out vec3 io_normal;
out vec3 io_frag_pos;
out vec3 io_light_pos;
out vec3 io_view_pos;

uniform mat4 u_projection;
uniform mat4 u_modelview_light;
uniform mat4 u_modelview_apple;
uniform vec4 u_light_pos;
uniform vec4 u_view_pos;

void main()
{
	mat3 normal_matrix = mat3(transpose(inverse(mat3(u_modelview_apple))));
	io_normal = normal_matrix * vec3(in_normal);
	
	//io_normal = vec3(mat3(u_modelview_apple) * vec3(in_normal));

	io_frag_pos  = vec3(u_modelview_apple * in_pos);
	io_light_pos = vec3(u_modelview_light * u_light_pos);
	io_view_pos  = vec3(u_modelview_light * u_view_pos);
	io_tex_coord = in_tex_coord;

	gl_Position = u_projection * u_modelview_apple * in_pos;
}
