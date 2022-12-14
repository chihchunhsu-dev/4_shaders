#version 410

in vec2 io_tex_coord;
in vec3 io_frag_pos;
in vec3 io_light_pos;
in vec3 io_normal;

out vec4 out_frag_color;

uniform sampler2D u_texture;

void main()
{
	vec4 color;
	vec3 normal = normalize(io_normal);
    vec3 light_dir = normalize(io_light_pos - io_frag_pos);
	float intensity = max(dot(normal, light_dir), 0.0);

	if		(intensity > 0.95) color = vec4(1.0, 1.0, 1.0, 1.0);
	else if (intensity > 0.75) color = vec4(0.8, 0.8, 0.8, 1.0);
	else if (intensity > 0.50) color = vec4(0.6, 0.6, 0.6, 1.0);
	else if (intensity > 0.25) color = vec4(0.4, 0.4, 0.4, 1.0);
	else					   color = vec4(0.2 ,0.2, 0.2, 1.0);

	out_frag_color = texture2D(u_texture, io_tex_coord) * color ;	
}