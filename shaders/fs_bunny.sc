$input v_normal, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_tex, 0);

void main()
{
	vec4 texel = texture2D(s_tex, v_texcoord0);
	vec3 lightDir = vec3(1.0, 1.0, 1.0);
	float diff = clamp(dot(v_normal, lightDir), 0.5, 1.0);
	gl_FragColor = diff * texel;
}