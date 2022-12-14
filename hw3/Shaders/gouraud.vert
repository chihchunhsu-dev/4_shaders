#version 410

layout(location = 0) in vec4 in_pos;
layout(location = 1) in vec4 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec4 io_color;

uniform mat4 u_projection;
uniform mat4 u_modelview_light;
uniform mat4 u_modelview_apple;
uniform vec4 u_light_pos;
uniform vec4 u_view_pos;
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform float u_shininess;
uniform sampler2D u_texture;

void main()
{
	mat3 normal_matrix = mat3(transpose(inverse(mat3(u_modelview_apple))));
	vec3 frag_pos  = vec3(u_modelview_apple * in_pos);
	vec3 light_pos = vec3(u_modelview_light * u_light_pos);
	vec3 view_pos  = vec3(u_modelview_light * u_view_pos);
	vec2 tex_coord = in_tex_coord;

	// ambient
	vec4 ambient = vec4(u_ka, 1.0);

	// diffuse
	vec3 normal = normalize(normal_matrix * vec3(in_normal));
	vec3 light_dir = normalize(light_pos - frag_pos);
	float diff = max(dot(normal, light_dir), 0.0);
	vec4 diffuse = texture2D(u_texture, tex_coord) * diff;

	// specular
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_shininess);
	vec4 specular = vec4(u_ks * spec, 1.0);

	io_color = ambient + diffuse + specular;

	gl_Position = u_projection * u_modelview_apple * in_pos;

}
