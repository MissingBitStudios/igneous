$input v_color0

#include <bgfx_shader.sh>

void main()
{
    gl_FragColor = vec4(v_color0.xyz, 1.0f);   
}
