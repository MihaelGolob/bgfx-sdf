typedef unsigned char uint8_t;
static const uint8_t fs_font_msdf_bench_glsl[865] =
{
	0x46, 0x53, 0x48, 0x0b, 0x01, 0x83, 0xf2, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0a, 0x73, // FSH............s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, // _texColor.......
	0x00, 0x00, 0x00, 0x39, 0x03, 0x00, 0x00, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, // ...9...varying v
	0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, // ec4 v_texcoord0;
	0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, // .uniform sampler
	0x32, 0x44, 0x20, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x76, // 2D s_texColor;.v
	0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, // oid main ().{.  
	0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, // vec4 tmpvar_1;. 
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, 0x74, //  tmpvar_1 = text
	0x75, 0x72, 0x65, 0x32, 0x44, 0x20, 0x28, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, // ure2D (s_texColo
	0x72, 0x2c, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x2e, 0x78, // r, v_texcoord0.x
	0x79, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x74, 0x6d, 0x70, 0x76, // y);.  float tmpv
	0x61, 0x72, 0x5f, 0x32, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, // ar_2;.  tmpvar_2
	0x20, 0x3d, 0x20, 0x28, 0x28, 0x28, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, //  = (((.    (tmpv
	0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x20, 0x2b, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // ar_1.x + tmpvar_
	0x31, 0x2e, 0x79, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x2b, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, // 1.y).   + tmpvar
	0x5f, 0x31, 0x2e, 0x7a, 0x29, 0x20, 0x2d, 0x20, 0x6d, 0x69, 0x6e, 0x20, 0x28, 0x74, 0x6d, 0x70, // _1.z) - min (tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x2c, 0x20, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6d, 0x69, // var_1.x, .    mi
	0x6e, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x79, 0x2c, 0x20, 0x74, // n (tmpvar_1.y, t
	0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x29, 0x0a, 0x20, 0x20, 0x29, 0x29, 0x20, // mpvar_1.z).  )) 
	0x2d, 0x20, 0x6d, 0x61, 0x78, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, // - max (tmpvar_1.
	0x78, 0x2c, 0x20, 0x6d, 0x61, 0x78, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, // x, max (tmpvar_1
	0x2e, 0x79, 0x2c, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x29, 0x29, // .y, tmpvar_1.z))
	0x29, 0x3b, 0x0a, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // );.  float tmpva
	0x72, 0x5f, 0x33, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x20, // r_3;.  tmpvar_3 
	0x3d, 0x20, 0x28, 0x28, 0x61, 0x62, 0x73, 0x28, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64, 0x46, 0x64, // = ((abs(.    dFd
	0x78, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, 0x29, 0x0a, 0x20, 0x20, 0x29, 0x20, // x(tmpvar_2).  ) 
	0x2b, 0x20, 0x61, 0x62, 0x73, 0x28, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64, 0x46, 0x64, 0x79, 0x28, // + abs(.    dFdy(
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, 0x29, 0x0a, 0x20, 0x20, 0x29, 0x29, 0x20, 0x2a, // tmpvar_2).  )) *
	0x20, 0x30, 0x2e, 0x37, 0x35, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, //  0.75);.  float 
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x3b, 0x0a, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, // tmpvar_4;.  floa
	0x74, 0x20, 0x65, 0x64, 0x67, 0x65, 0x30, 0x5f, 0x35, 0x3b, 0x0a, 0x20, 0x20, 0x65, 0x64, 0x67, // t edge0_5;.  edg
	0x65, 0x30, 0x5f, 0x35, 0x20, 0x3d, 0x20, 0x28, 0x30, 0x2e, 0x35, 0x20, 0x2d, 0x20, 0x74, 0x6d, // e0_5 = (0.5 - tm
	0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // pvar_3);.  float
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, //  tmpvar_6;.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x36, 0x20, 0x3d, 0x20, 0x63, 0x6c, 0x61, 0x6d, 0x70, 0x20, 0x28, 0x28, // var_6 = clamp ((
	0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, 0x20, 0x2d, 0x20, 0x65, 0x64, 0x67, 0x65, // (tmpvar_2 - edge
	0x30, 0x5f, 0x35, 0x29, 0x20, 0x2f, 0x20, 0x28, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x28, 0x30, 0x2e, // 0_5) / (.    (0.
	0x35, 0x20, 0x2b, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x29, 0x0a, 0x20, 0x20, // 5 + tmpvar_3).  
	0x20, 0x2d, 0x20, 0x65, 0x64, 0x67, 0x65, 0x30, 0x5f, 0x35, 0x29, 0x29, 0x2c, 0x20, 0x30, 0x2e, //  - edge0_5)), 0.
	0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // 0, 1.0);.  tmpva
	0x72, 0x5f, 0x34, 0x20, 0x3d, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x20, // r_4 = (tmpvar_6 
	0x2a, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x20, 0x2a, 0x20, 0x28, 0x33, // * (tmpvar_6 * (3
	0x2e, 0x30, 0x20, 0x2d, 0x20, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x28, 0x32, 0x2e, 0x30, 0x20, 0x2a, // .0 - .    (2.0 *
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x29, 0x0a, 0x20, 0x20, 0x29, 0x29, 0x29, //  tmpvar_6).  )))
	0x3b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // ;.  vec4 tmpvar_
	0x37, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x2e, 0x77, 0x20, // 7;.  tmpvar_7.w 
	0x3d, 0x20, 0x31, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // = 1.0;.  tmpvar_
	0x37, 0x2e, 0x78, 0x20, 0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x3b, 0x0a, // 7.x = tmpvar_4;.
	0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x2e, 0x79, 0x20, 0x3d, 0x20, 0x74, //   tmpvar_7.y = t
	0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // mpvar_4;.  tmpva
	0x72, 0x5f, 0x37, 0x2e, 0x7a, 0x20, 0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, // r_7.z = tmpvar_4
	0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, // ;.  gl_FragColor
	0x20, 0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, //  = tmpvar_7;.}..
	0x00,                                                                                           // .
};
typedef unsigned char uint8_t;
static const uint8_t fs_font_msdf_bench_essl[909] =
{
	0x46, 0x53, 0x48, 0x0b, 0x01, 0x83, 0xf2, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0a, 0x73, // FSH............s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, // _texColor.......
	0x00, 0x00, 0x00, 0x65, 0x03, 0x00, 0x00, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x68, // ...e...varying h
	0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, // ighp vec4 v_texc
	0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, // oord0;.uniform s
	0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x32, 0x44, 0x20, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, // ampler2D s_texCo
	0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, // lor;.void main (
	0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x6c, 0x6f, 0x77, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, // ).{.  lowp vec4 
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, // tmpvar_1;.  tmpv
	0x61, 0x72, 0x5f, 0x31, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x32, 0x44, // ar_1 = texture2D
	0x20, 0x28, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x2c, 0x20, 0x76, 0x5f, //  (s_texColor, v_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x2e, 0x78, 0x79, 0x29, 0x3b, 0x0a, 0x20, // texcoord0.xy);. 
	0x20, 0x6c, 0x6f, 0x77, 0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x74, 0x6d, 0x70, 0x76, //  lowp float tmpv
	0x61, 0x72, 0x5f, 0x32, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, // ar_2;.  tmpvar_2
	0x20, 0x3d, 0x20, 0x28, 0x28, 0x28, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, //  = (((.    (tmpv
	0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x20, 0x2b, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // ar_1.x + tmpvar_
	0x31, 0x2e, 0x79, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x2b, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, // 1.y).   + tmpvar
	0x5f, 0x31, 0x2e, 0x7a, 0x29, 0x20, 0x2d, 0x20, 0x6d, 0x69, 0x6e, 0x20, 0x28, 0x74, 0x6d, 0x70, // _1.z) - min (tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x2c, 0x20, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6d, 0x69, // var_1.x, .    mi
	0x6e, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x79, 0x2c, 0x20, 0x74, // n (tmpvar_1.y, t
	0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x29, 0x0a, 0x20, 0x20, 0x29, 0x29, 0x20, // mpvar_1.z).  )) 
	0x2d, 0x20, 0x6d, 0x61, 0x78, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, // - max (tmpvar_1.
	0x78, 0x2c, 0x20, 0x6d, 0x61, 0x78, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, // x, max (tmpvar_1
	0x2e, 0x79, 0x2c, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x29, 0x29, // .y, tmpvar_1.z))
	0x29, 0x3b, 0x0a, 0x20, 0x20, 0x6c, 0x6f, 0x77, 0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, // );.  lowp float 
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, // tmpvar_3;.  tmpv
	0x61, 0x72, 0x5f, 0x33, 0x20, 0x3d, 0x20, 0x28, 0x28, 0x61, 0x62, 0x73, 0x28, 0x0a, 0x20, 0x20, // ar_3 = ((abs(.  
	0x20, 0x20, 0x64, 0x46, 0x64, 0x78, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, 0x29, //   dFdx(tmpvar_2)
	0x0a, 0x20, 0x20, 0x29, 0x20, 0x2b, 0x20, 0x61, 0x62, 0x73, 0x28, 0x0a, 0x20, 0x20, 0x20, 0x20, // .  ) + abs(.    
	0x64, 0x46, 0x64, 0x79, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, 0x29, 0x0a, 0x20, // dFdy(tmpvar_2). 
	0x20, 0x29, 0x29, 0x20, 0x2a, 0x20, 0x30, 0x2e, 0x37, 0x35, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x6c, //  )) * 0.75);.  l
	0x6f, 0x77, 0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, // owp float tmpvar
	0x5f, 0x34, 0x3b, 0x0a, 0x20, 0x20, 0x6c, 0x6f, 0x77, 0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // _4;.  lowp float
	0x20, 0x65, 0x64, 0x67, 0x65, 0x30, 0x5f, 0x35, 0x3b, 0x0a, 0x20, 0x20, 0x65, 0x64, 0x67, 0x65, //  edge0_5;.  edge
	0x30, 0x5f, 0x35, 0x20, 0x3d, 0x20, 0x28, 0x30, 0x2e, 0x35, 0x20, 0x2d, 0x20, 0x74, 0x6d, 0x70, // 0_5 = (0.5 - tmp
	0x76, 0x61, 0x72, 0x5f, 0x33, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x6c, 0x6f, 0x77, 0x70, 0x20, 0x66, // var_3);.  lowp f
	0x6c, 0x6f, 0x61, 0x74, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x3b, 0x0a, 0x20, // loat tmpvar_6;. 
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x20, 0x3d, 0x20, 0x63, 0x6c, 0x61, 0x6d, //  tmpvar_6 = clam
	0x70, 0x20, 0x28, 0x28, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, 0x20, 0x2d, 0x20, // p (((tmpvar_2 - 
	0x65, 0x64, 0x67, 0x65, 0x30, 0x5f, 0x35, 0x29, 0x20, 0x2f, 0x20, 0x28, 0x0a, 0x20, 0x20, 0x20, // edge0_5) / (.   
	0x20, 0x28, 0x30, 0x2e, 0x35, 0x20, 0x2b, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, //  (0.5 + tmpvar_3
	0x29, 0x0a, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x65, 0x64, 0x67, 0x65, 0x30, 0x5f, 0x35, 0x29, 0x29, // ).   - edge0_5))
	0x2c, 0x20, 0x30, 0x2e, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, // , 0.0, 1.0);.  t
	0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x20, 0x3d, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, // mpvar_4 = (tmpva
	0x72, 0x5f, 0x36, 0x20, 0x2a, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x20, // r_6 * (tmpvar_6 
	0x2a, 0x20, 0x28, 0x33, 0x2e, 0x30, 0x20, 0x2d, 0x20, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x28, 0x32, // * (3.0 - .    (2
	0x2e, 0x30, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x36, 0x29, 0x0a, 0x20, // .0 * tmpvar_6). 
	0x20, 0x29, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, //  )));.  mediump 
	0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x3b, 0x0a, 0x20, // vec4 tmpvar_7;. 
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x31, 0x2e, //  tmpvar_7.w = 1.
	0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x2e, 0x78, 0x20, // 0;.  tmpvar_7.x 
	0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, // = tmpvar_4;.  tm
	0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x2e, 0x79, 0x20, 0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // pvar_7.y = tmpva
	0x72, 0x5f, 0x34, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x2e, // r_4;.  tmpvar_7.
	0x7a, 0x20, 0x3d, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x3b, 0x0a, 0x20, 0x20, // z = tmpvar_4;.  
	0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x74, // gl_FragColor = t
	0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x37, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                   // mpvar_7;.}...
};
typedef unsigned char uint8_t;
static const uint8_t fs_font_msdf_bench_spv[1239] =
{
	0x46, 0x53, 0x48, 0x0b, 0x01, 0x83, 0xf2, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0a, 0x73, // FSH............s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, // _texColor0......
	0x02, 0x22, 0x00, 0xac, 0x04, 0x00, 0x00, 0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x0b, // .".......#......
	0x00, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, // ................
	0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, 0x4c, 0x2e, // ...........GLSL.
	0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, // std.450.........
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, // ................
	0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x99, // ...main.........
	0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03, // ................
	0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x04, // ................
	0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x29, // ...main........)
	0x00, 0x00, 0x00, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, // ...s_texColorSam
	0x70, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x73, // pler.......,...s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, // _texColorTexture
	0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x76, 0x5f, 0x74, 0x65, 0x78, // ...........v_tex
	0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x00, 0x05, 0x00, 0x06, 0x00, 0x99, 0x00, 0x00, 0x00, 0x62, // coord0.........b
	0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x00, 0x00, 0x47, // gfx_FragData0..G
	0x00, 0x04, 0x00, 0x29, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...)...".......G
	0x00, 0x04, 0x00, 0x29, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x47, // ...)...!.......G
	0x00, 0x04, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...,...".......G
	0x00, 0x04, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, // ...,...!.......G
	0x00, 0x04, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, // ...............G
	0x00, 0x04, 0x00, 0x99, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, // ................
	0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, // .......!........
	0x00, 0x00, 0x00, 0x1a, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x07, // ................
	0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x08, 0x00, 0x00, 0x00, 0x07, // ... ............
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0b, // ................
	0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0d, // ................
	0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x28, // ........... ...(
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x28, // ...........;...(
	0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x2b, // ...)....... ...+
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x2b, // ...........;...+
	0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x03, 0x00, 0x38, // ...,...........8
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x75, // .......+.......u
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3f, 0x2b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x78, // .....@?+.......x
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x2b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x82, // ......?+........
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x20, 0x00, 0x04, 0x00, 0x85, 0x00, 0x00, 0x00, 0x01, // ......? ........
	0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x85, 0x00, 0x00, 0x00, 0x8c, // .......;........
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x98, 0x00, 0x00, 0x00, 0x03, // ....... ........
	0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x98, 0x00, 0x00, 0x00, 0x99, // .......;........
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, // .......6........
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x05, // ................
	0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x29, // ...=.......*...)
	0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x2c, // ...=.......-...,
	0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00, 0x8c, // ...=............
	0x00, 0x00, 0x00, 0x4f, 0x00, 0x07, 0x00, 0x0b, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x8d, // ...O............
	0x00, 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x56, // ...............V
	0x00, 0x05, 0x00, 0x38, 0x00, 0x00, 0x00, 0xe2, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x2a, // ...8.......-...*
	0x00, 0x00, 0x00, 0x57, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0xe2, // ...W............
	0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xc2, // .......Q........
	0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x07, // ...........Q....
	0x00, 0x00, 0x00, 0xc4, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x51, // ...............Q
	0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x02, // ................
	0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xe9, 0x00, 0x00, 0x00, 0xc2, // ................
	0x00, 0x00, 0x00, 0xc4, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xeb, // ................
	0x00, 0x00, 0x00, 0xe9, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x07, // ................
	0x00, 0x00, 0x00, 0xef, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0xc4, // ...........%....
	0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0xf0, // ................
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0xef, // .......%........
	0x00, 0x00, 0x00, 0x83, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xf1, 0x00, 0x00, 0x00, 0xeb, // ................
	0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0xf5, // ................
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0xc4, 0x00, 0x00, 0x00, 0xc6, // .......(........
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0xf6, 0x00, 0x00, 0x00, 0x01, // ................
	0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x00, 0x00, 0x83, // ...(............
	0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x00, 0x00, 0xf1, 0x00, 0x00, 0x00, 0xf6, // ................
	0x00, 0x00, 0x00, 0xd1, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0xc9, 0x00, 0x00, 0x00, 0xf7, // ................
	0x00, 0x00, 0x00, 0x7f, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xc9, // ................
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x01, // ................
	0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x78, // ...2.......u...x
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, 0x01, // ................
	0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0xc9, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x78, // ...2.......u...x
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x01, // ................
	0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, 0xf7, // ...1............
	0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00, 0x0d, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0xd0, // ...P............
	0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x3e, // ...............>
	0x00, 0x03, 0x00, 0x99, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, // ...............8
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,                                                       // .......
};
typedef unsigned char uint8_t;
static const uint8_t fs_font_msdf_bench_dx11[1000] =
{
	0x46, 0x53, 0x48, 0x0b, 0x01, 0x83, 0xf2, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0a, 0x73, // FSH............s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, // _texColor0......
	0x00, 0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x01, // ....s_texColor0.
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x03, 0x00, 0x00, 0x44, 0x58, 0x42, 0x43, // ............DXBC
	0x1f, 0xec, 0xf0, 0x7b, 0x91, 0xdf, 0xd3, 0x4d, 0x96, 0xf9, 0x2b, 0x3c, 0xb0, 0xcc, 0x9e, 0x53, // ...{...M..+<...S
	0x01, 0x00, 0x00, 0x00, 0xa8, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, // ............,...
	0xa0, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0x49, 0x53, 0x47, 0x4e, 0x6c, 0x00, 0x00, 0x00, // ........ISGNl...
	0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........P.......
	0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, // ................
	0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
	0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........b.......
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x03, 0x00, 0x00, // ................
	0x53, 0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, // SV_POSITION.COLO
	0x52, 0x00, 0x54, 0x45, 0x58, 0x43, 0x4f, 0x4f, 0x52, 0x44, 0x00, 0xab, 0x4f, 0x53, 0x47, 0x4e, // R.TEXCOORD..OSGN
	0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, // ,........... ...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x54, 0x41, 0x52, 0x47, 0x45, 0x54, 0x00, 0xab, 0xab, // ....SV_TARGET...
	0x53, 0x48, 0x45, 0x58, 0xcc, 0x02, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0xb3, 0x00, 0x00, 0x00, // SHEX....P.......
	0x6a, 0x08, 0x00, 0x01, 0x5a, 0x00, 0x00, 0x03, 0x00, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // j...Z....`......
	0x58, 0x18, 0x00, 0x04, 0x00, 0x70, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, // X....p......UU..
	0x62, 0x10, 0x00, 0x03, 0x32, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, // b...2.......e...
	0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, // . ......h.......
	0x45, 0x00, 0x00, 0x8b, 0xc2, 0x00, 0x00, 0x80, 0x43, 0x55, 0x15, 0x00, 0x72, 0x00, 0x10, 0x00, // E.......CU..r...
	0x00, 0x00, 0x00, 0x00, 0x46, 0x10, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x46, 0x7e, 0x10, 0x00, // ....F.......F~..
	0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, // .....`..........
	0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x0a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x82, 0x00, 0x10, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x10, 0x00, // ....*.......:...
	0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x07, 0x12, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, // ....3...........
	0x2a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // *...............
	0x33, 0x00, 0x00, 0x07, 0x12, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, // 3...............
	0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, // ................
	0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ........:.......
	0x0a, 0x00, 0x10, 0x80, 0x41, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x07, // ....A.......4...
	0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ".......*.......
	0x1a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x07, 0x12, 0x00, 0x10, 0x00, // ........4.......
	0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x12, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x0a, 0x00, 0x10, 0x80, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x10, 0x00, // ....A.......:...
	0x00, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, 0x05, 0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ....z...".......
	0x0a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x05, 0x42, 0x00, 0x10, 0x00, // ........|...B...
	0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, // ................
	0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x10, 0x80, 0x81, 0x00, 0x00, 0x00, // ".......*.......
	0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x10, 0x80, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x32, 0x00, 0x00, 0x09, 0x42, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x10, 0x00, // 2...B...........
	0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3f, 0x01, 0x40, 0x00, 0x00, // .....@....@?.@..
	0x00, 0x00, 0x00, 0x3f, 0x32, 0x00, 0x00, 0x0a, 0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ...?2...".......
	0x1a, 0x00, 0x10, 0x80, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, // ....A........@..
	0x00, 0x00, 0x40, 0x3f, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x08, // ..@?.@.....?....
	0x52, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x05, 0x10, 0x80, 0x41, 0x00, 0x00, 0x00, // R.......V...A...
	0x00, 0x00, 0x00, 0x00, 0x06, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x0a, // ................
	0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, // "........@.....?
	0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f, 0x2a, 0x00, 0x10, 0x00, // ...?...?...?*...
	0x00, 0x00, 0x00, 0x00, 0x38, 0x20, 0x00, 0x07, 0x12, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ....8 ..........
	0x1a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x32, 0x00, 0x00, 0x09, 0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, // 2..."...........
	0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x40, 0x00, 0x00, // .....@.......@..
	0x00, 0x00, 0x40, 0x40, 0x38, 0x00, 0x00, 0x07, 0x12, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ..@@8...........
	0x0a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x38, 0x00, 0x00, 0x07, 0x72, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x10, 0x00, // 8...r ..........
	0x00, 0x00, 0x00, 0x00, 0x56, 0x05, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, // ....V.......6...
	0x82, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, // . .......@.....?
	0x3e, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,                                                 // >.......
};
typedef unsigned char uint8_t;
static const uint8_t fs_font_msdf_bench_mtl[956] =
{
	0x46, 0x53, 0x48, 0x0b, 0x01, 0x83, 0xf2, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x11, 0x73, // FSH............s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, // _texColorSampler
	0x11, 0x01, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x73, 0x5f, 0x74, 0x65, 0x78, // ...........s_tex
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x11, 0x01, 0xff, 0xff, // ColorTexture....
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, // .......s_texColo
	0x72, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x03, 0x00, 0x00, 0x23, // r..........Y...#
	0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x20, 0x3c, 0x6d, 0x65, 0x74, 0x61, 0x6c, 0x5f, 0x73, // include <metal_s
	0x74, 0x64, 0x6c, 0x69, 0x62, 0x3e, 0x0a, 0x23, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x20, // tdlib>.#include 
	0x3c, 0x73, 0x69, 0x6d, 0x64, 0x2f, 0x73, 0x69, 0x6d, 0x64, 0x2e, 0x68, 0x3e, 0x0a, 0x0a, 0x75, // <simd/simd.h>..u
	0x73, 0x69, 0x6e, 0x67, 0x20, 0x6e, 0x61, 0x6d, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x20, 0x6d, // sing namespace m
	0x65, 0x74, 0x61, 0x6c, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x78, 0x6c, // etal;..struct xl
	0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x0a, 0x7b, 0x0a, // atMtlMain_out.{.
	0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, //     float4 bgfx_
	0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, // FragData0 [[colo
	0x72, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, // r(0)]];.};..stru
	0x63, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x69, // ct xlatMtlMain_i
	0x6e, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x76, // n.{.    float4 v
	0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x5b, 0x5b, 0x75, 0x73, 0x65, // _texcoord0 [[use
	0x72, 0x28, 0x6c, 0x6f, 0x63, 0x6e, 0x31, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, // r(locn1)]];.};..
	0x66, 0x72, 0x61, 0x67, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, // fragment xlatMtl
	0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, // Main_out xlatMtl
	0x4d, 0x61, 0x69, 0x6e, 0x28, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, // Main(xlatMtlMain
	0x5f, 0x69, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x5b, 0x5b, 0x73, 0x74, 0x61, 0x67, 0x65, 0x5f, 0x69, // _in in [[stage_i
	0x6e, 0x5d, 0x5d, 0x2c, 0x20, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x32, 0x64, 0x3c, 0x66, // n]], texture2d<f
	0x6c, 0x6f, 0x61, 0x74, 0x3e, 0x20, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, // loat> s_texColor
	0x20, 0x5b, 0x5b, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x2c, //  [[texture(0)]],
	0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x20, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, //  sampler s_texCo
	0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x20, 0x5b, 0x5b, 0x73, 0x61, 0x6d, // lorSampler [[sam
	0x70, 0x6c, 0x65, 0x72, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, // pler(0)]]).{.   
	0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, //  xlatMtlMain_out
	0x20, 0x6f, 0x75, 0x74, 0x20, 0x3d, 0x20, 0x7b, 0x7d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, //  out = {};.    f
	0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x5f, 0x32, 0x32, 0x35, 0x20, 0x3d, 0x20, 0x73, 0x5f, 0x74, // loat4 _225 = s_t
	0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x28, 0x73, // exColor.sample(s
	0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, // _texColorSampler
	0x2c, 0x20, 0x69, 0x6e, 0x2e, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, // , in.v_texcoord0
	0x2e, 0x78, 0x79, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, // .xy);.    float 
	0x5f, 0x31, 0x39, 0x31, 0x20, 0x3d, 0x20, 0x5f, 0x32, 0x32, 0x35, 0x2e, 0x78, 0x3b, 0x0a, 0x20, // _191 = _225.x;. 
	0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x5f, 0x31, 0x39, 0x33, 0x20, 0x3d, 0x20, //    float _193 = 
	0x5f, 0x32, 0x32, 0x35, 0x2e, 0x79, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, // _225.y;.    floa
	0x74, 0x20, 0x5f, 0x31, 0x39, 0x35, 0x20, 0x3d, 0x20, 0x5f, 0x32, 0x32, 0x35, 0x2e, 0x7a, 0x3b, // t _195 = _225.z;
	0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x5f, 0x32, 0x34, 0x34, 0x20, // .    float _244 
	0x3d, 0x20, 0x28, 0x28, 0x28, 0x5f, 0x31, 0x39, 0x31, 0x20, 0x2b, 0x20, 0x5f, 0x31, 0x39, 0x33, // = (((_191 + _193
	0x29, 0x20, 0x2b, 0x20, 0x5f, 0x31, 0x39, 0x35, 0x29, 0x20, 0x2d, 0x20, 0x66, 0x61, 0x73, 0x74, // ) + _195) - fast
	0x3a, 0x3a, 0x6d, 0x69, 0x6e, 0x28, 0x5f, 0x31, 0x39, 0x31, 0x2c, 0x20, 0x66, 0x61, 0x73, 0x74, // ::min(_191, fast
	0x3a, 0x3a, 0x6d, 0x69, 0x6e, 0x28, 0x5f, 0x31, 0x39, 0x33, 0x2c, 0x20, 0x5f, 0x31, 0x39, 0x35, // ::min(_193, _195
	0x29, 0x29, 0x29, 0x20, 0x2d, 0x20, 0x66, 0x61, 0x73, 0x74, 0x3a, 0x3a, 0x6d, 0x61, 0x78, 0x28, // ))) - fast::max(
	0x5f, 0x31, 0x39, 0x31, 0x2c, 0x20, 0x66, 0x61, 0x73, 0x74, 0x3a, 0x3a, 0x6d, 0x61, 0x78, 0x28, // _191, fast::max(
	0x5f, 0x31, 0x39, 0x33, 0x2c, 0x20, 0x5f, 0x31, 0x39, 0x35, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, // _193, _195));.  
	0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x5f, 0x31, 0x39, 0x38, 0x20, 0x3d, 0x20, 0x66, //   float _198 = f
	0x77, 0x69, 0x64, 0x74, 0x68, 0x28, 0x5f, 0x32, 0x34, 0x34, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, // width(_244);.   
	0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x5f, 0x32, 0x30, 0x35, 0x20, 0x3d, 0x20, 0x73, 0x6d, //  float _205 = sm
	0x6f, 0x6f, 0x74, 0x68, 0x73, 0x74, 0x65, 0x70, 0x28, 0x66, 0x6d, 0x61, 0x28, 0x2d, 0x5f, 0x31, // oothstep(fma(-_1
	0x39, 0x38, 0x2c, 0x20, 0x30, 0x2e, 0x37, 0x35, 0x2c, 0x20, 0x30, 0x2e, 0x35, 0x29, 0x2c, 0x20, // 98, 0.75, 0.5), 
	0x66, 0x6d, 0x61, 0x28, 0x5f, 0x31, 0x39, 0x38, 0x2c, 0x20, 0x30, 0x2e, 0x37, 0x35, 0x2c, 0x20, // fma(_198, 0.75, 
	0x30, 0x2e, 0x35, 0x29, 0x2c, 0x20, 0x5f, 0x32, 0x34, 0x34, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, // 0.5), _244);.   
	0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, //  out.bgfx_FragDa
	0x74, 0x61, 0x30, 0x20, 0x3d, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x28, 0x5f, 0x32, 0x30, // ta0 = float4(_20
	0x35, 0x2c, 0x20, 0x5f, 0x32, 0x30, 0x35, 0x2c, 0x20, 0x5f, 0x32, 0x30, 0x35, 0x2c, 0x20, 0x31, // 5, _205, _205, 1
	0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, // .0);.    return 
	0x6f, 0x75, 0x74, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00, 0x00, 0x20, 0x00,                         // out;.}.... .
};
extern const uint8_t* fs_font_msdf_bench_pssl;
extern const uint32_t fs_font_msdf_bench_pssl_size;
