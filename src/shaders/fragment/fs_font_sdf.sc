$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLERCUBE(s_texColor, 0);

void main() {
	vec4 color = textureCube(s_texColor, v_texcoord0.xyz);
		
	float threshold = 0.5;
	vec4 out_color = color.r > threshold ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
	
	gl_FragColor = out_color;
}
