$input a_position, a_normal, a_texcoord0, a_color0, a_tangent, a_bitangent
$output v_texcoord0, v_normal, v_color0, v_fogamount

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;
	v_normal = a_normal;
	v_color0 = a_color0;
	v_fogamount = 1.0 - clamp((100.0 - length(gl_Position.xyz)) / (100.0 - 90.0), 0.0, 1.0);
}