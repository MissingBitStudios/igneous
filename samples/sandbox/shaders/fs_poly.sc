$input v_color0, v_normal

#include <bgfx_shader.sh>

void main()
{
	vec3 lightDir = vec3(1.0, 1.0, 1.0);
	float diff = clamp(dot(v_normal, lightDir), 0.5, 1.0);
	gl_FragColor = vec4(diff * v_color0.xyz, v_color0.a);
}