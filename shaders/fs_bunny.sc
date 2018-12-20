$input v_texcoord0, v_normal, v_color0, v_fogamount

#include <bgfx_shader.sh>

SAMPLER2D(s_tex, 0);
SAMPLER2D(s_norm, 1);

void main()
{
	vec4 texel = texture2D(s_tex, v_texcoord0);
	vec3 lightDir = vec3(1.0, 1.0, 1.0);
	float diff = clamp(dot(v_normal, lightDir), 0.5, 1.0);
	gl_FragColor = mix(v_color0 * vec4(diff * texel.xyz, texel.a), vec4(1.0, 1.0, 1.0, 1.0), v_fogamount);
}