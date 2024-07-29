$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLERCUBE(s_texColor, 0);

float median(float a, float b, float c) {
    return a + b + c - min(a, min(b, c)) - max(a, max(b, c));
}

void main() {
    vec4 color = textureCube(s_texColor, v_texcoord0.xyz);
    float d = median(color.r, color.g, color.b) - 0.5;
    float w = clamp(d / fwidth(d) + 0.5, 0.0, 1.0);
    
    vec4 outside_color = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 inside_color = vec4(1.0, 1.0, 1.0, 1.0);
    
    gl_FragColor = mix(outside_color, inside_color, w);
}
