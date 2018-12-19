$input a_position, a_normal, a_texcoord0, a_color0, a_tangent, a_bitangent
$output v_normal, v_texcoord0

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;
	v_normal = a_normal;
}