$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLERCUBE(s_texColor, 0);

void main() {
    vec4 color = textureCube(s_texColor, v_texcoord0.xyz);
    gl_FragColor = vec4(color);
}
