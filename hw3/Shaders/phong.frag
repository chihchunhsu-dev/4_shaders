#version 410

in vec2 io_tex_coord;
in vec3 io_normal;
in vec3 io_frag_pos;
in vec3 io_light_pos;
in vec3 io_view_pos;

out vec4 out_frag_color;

uniform sampler2D u_texture;
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform float u_shininess;

void main()
{
	// ambient
	vec4 ambient = vec4(u_ka, 1.0);

	// diffuse
	vec3 normal = normalize(io_normal);
    vec3 light_dir = normalize(io_light_pos - io_frag_pos);
	float diff = max(dot(normal, light_dir), 0.0);
	vec4 diffuse = texture2D(u_texture, io_tex_coord) * diff;

	// specular
	vec3 view_dir = normalize(io_view_pos - io_frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_shininess);
	vec4 specular = vec4(u_ks * spec, 1.0);

	out_frag_color = ambient + diffuse + specular;
}