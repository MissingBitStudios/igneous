$input a_position, a_normal, a_texcoord0, a_tangent, a_bitangent
$output v_color0

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	if(a_normal.x > 0.5)
	{
		v_color0 = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		v_color0 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}