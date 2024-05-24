$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLERCUBE(s_texColor, 0);

void main() {
	vec4 color = textureCube(s_texColor, v_texcoord0.xyz);
    int index = int(v_texcoord0.w*4.0 + 0.5);
    float alpha = index < 1 ? color.z :
                  index < 2 ? color.y :
                  index < 3 ? color.x : color.w;
        
    float threshold = 0.5;
    vec4 out_color = vec4(0,0,0,0);
    
    if (alpha > 0.5) {
        out_color = vec4(1.0, 1.0, 1.0, 1.0);
    } else if (alpha > 0.4) {
        out_color = vec4(0.8, 0.8, 0.0, 1.0);
    } else {
        out_color = vec4(0.0, 0.0, 0.0, 0.0);
    }
    
    gl_FragColor = out_color;
}
