$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLER2D(s_texColor, 0);

float median(float a, float b, float c) {
    return a + b + c - min(a, min(b, c)) - max(a, max(b, c));
}

void main() {
    vec4 color = texture2D(s_texColor, v_texcoord0.xy);
    float d = median(color.r, color.g, color.b);
    
    float aaf = fwidth(d) * 0.75;
    float alpha = smoothstep(0.5 - aaf, 0.5 + aaf, d);
    
    gl_FragColor = vec4(alpha, alpha, alpha, 1.0);
}
