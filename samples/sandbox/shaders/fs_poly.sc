$input v_normal

#include <bgfx_shader.sh>

uniform vec4 u_color;

void main()
{
	vec3 lightDir = vec3(1.0, 1.0, 1.0);
	float diff = clamp(dot(v_normal, lightDir), 0.5, 1.0);
	gl_FragColor = vec4(diff * u_color.xyz, u_color.a);
}
