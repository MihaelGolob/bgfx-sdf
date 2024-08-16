$input v_color0, v_texcoord0

#include "../common/common.sh"

SAMPLERCUBE(s_texColor, 0);

void main() {
    vec4 out_color = vec4(1,1,1,1);
	vec4 color = textureCube(s_texColor, v_texcoord0.xyz);
	float d = color.b;
    float smoothing = fwidth(d);
        
    float outline_width = 0.07;
    float outer_edge_center = 0.5 - outline_width;
    
    float alpha = smoothstep(outer_edge_center - smoothing, outer_edge_center + smoothing, d);
    float border = smoothstep(0.5 - smoothing, 0.5 + smoothing, d);
    
    vec3 fill_color = vec3(1.0, 1.0, 1.0);
    vec3 outline_color = vec3(1.0, 0.4, 0.0);
    
    vec3 albedo = mix(outline_color, fill_color, border);
    gl_FragColor = vec4(albedo, alpha);
}
