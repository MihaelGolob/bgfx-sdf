$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLER2D(s_texColor, 0);

void main() {
    vec4 out_color = vec4(1,1,1,1);
	vec4 color = texture2D(s_texColor, v_texcoord0.xy);
	float d = color.r;
    
    float aaf = fwidth(d);
    float alpha = smoothstep(0.5 - aaf, 0.5 + aaf, d);
    
    gl_FragColor = vec4(out_color.rgb, alpha);
}
