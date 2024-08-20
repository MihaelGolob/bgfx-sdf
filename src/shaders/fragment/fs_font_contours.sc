$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLERCUBE(s_texColor, 0);

float median(float a, float b, float c) {
    return a + b + c - min(a, min(b, c)) - max(a, max(b, c));
}

void main() {
    vec4 color = textureCube(s_texColor, v_texcoord0.xyz);
    float d = - (median(color.r, color.g, color.b) - 0.5);
    
    vec3 col = (d>0.0) ? vec3(1.0, 1.0, 1.0) : vec3(0.65,0.85,1.0);
    col *= 1.0 - exp(-100.0*abs(d));
    col *= 0.8 + 0.2*cos(200.0*d); 
    float alpha = d < 0.0 ? 1.0 : (1.0 - 0.8 + 0.2*cos(200.0*d));
    
    gl_FragColor = vec4(col, alpha);
}
