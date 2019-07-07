$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_splash, 0);

void main()
{
	vec4 texel = texture2D(s_splash, v_texcoord0);
	gl_FragColor = texel;
}