vec4 v_color0    : COLOR0    = vec4(1.0, 0.0, 0.0, 1.0);
vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);
vec2 v_screenPos : TEXCOORD0 = vec2(0.0, 0.0);
vec3 v_skyColor  : TEXCOORD1 = vec3(0.0, 0.0, 1.0);
vec3 v_viewDir   : TEXCOORD2 = vec3(0.0, 0.0, 1.0);
vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 0.0);
float v_fogamount: PSIZE1    = float(0.0);

vec3 a_position  : POSITION;
vec3 a_normal    : NORMAL;
vec2 a_texcoord0 : TEXCOORD0;
vec3 a_tangent   : TANGENT;
vec3 a_bitangent : BITANGENT;
vec4 a_color0    : COLOR0;