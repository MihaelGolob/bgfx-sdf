$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLER2D(s_texColor, 0);

void main() {
	vec4 color = texture2D(s_texColor, v_texcoord0.xy);
	
	gl_FragColor = vec4(color.r, color.r, color.r, color.r);
}
