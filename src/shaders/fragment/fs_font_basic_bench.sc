$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLER2D(s_texColor, 0);

void main()
{
	vec4 color = texture2D(s_texColor, v_texcoord0.xy);
	int index = int(v_texcoord0.w*4.0 + 0.5);
	float alpha = index < 1 ? color.z :
		index < 2 ? color.y :
		index < 3 ? color.x : color.w;
	gl_FragColor = vec4(v_color0.xyz, v_color0.a * alpha);
	
	gl_FragColor = color;
}
