#version 400
#define PHOSPHOR 1.0
#define InputGamma 2.4
#define OutputGamma 2.2
#define SHARPNESS 1.0
#define COLOR_BOOST 1.5
#define RED_BOOST 1.0
#define GREEN_BOOST 1.0
#define BLUE_BOOST 1.0
#define SCANLINES_STRENGTH 0.72
#define BEAM_MIN_WIDTH 0.86
#define BEAM_MAX_WIDTH 1.0
#define CRT_ANTI_RINGING 0.8

#define GAMMA_IN(color)     pow(color, vec3(InputGamma, InputGamma, InputGamma))
#define GAMMA_OUT(color)    pow(color, vec3(1.0 / OutputGamma, 1.0 / OutputGamma, 1.0 / OutputGamma))

// Change these params to configure the horizontal filter.
const float  B =  0.0;
const float  C =  0.5;

const  mat4 invX = mat4(                          (-B - 6.0*C)/6.0,   (12.0 - 9.0*B - 6.0*C)/6.0,  -(12.0 - 9.0*B - 6.0*C)/6.0,   (B + 6.0*C)/6.0,
                                              (3.0*B + 12.0*C)/6.0, (-18.0 + 12.0*B + 6.0*C)/6.0, (18.0 - 15.0*B - 12.0*C)/6.0,                -C,
                                              (-3.0*B - 6.0*C)/6.0,                          0.0,          (3.0*B + 6.0*C)/6.0,               0.0,
                                                             B/6.0,            (6.0 - 2.0*B)/6.0,                        B/6.0,               0.0);

#define texCoord TEX0

#if __VERSION__ >= 130
#define IN in
#define tex2D texture
out vec4 FragColor;
#else
#define IN varying
#define FragColor gl_FragColor
#define tex2D texture2D
#endif

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#define PRECISION mediump
#else
#define PRECISION
#endif

uniform int FrameDirection;
uniform int FrameCount;
uniform PRECISION vec2 OutputSize;
uniform PRECISION vec2 TextureSize;
uniform PRECISION vec2 InputSize;
uniform sampler2D s_p;
IN vec2 texCoord;


void main()
{
    vec2 texture_size = vec2(SHARPNESS*TextureSize.x, TextureSize.y);

    vec3 color;
    vec2 dx = vec2(1.0/texture_size.x, 0.0);
    vec2 dy = vec2(0.0, 1.0/texture_size.y);
    vec2 pix_coord = texCoord*texture_size+vec2(-0.5,0.5);

    vec2 tc = (floor(pix_coord)+vec2(0.5,0.5))/texture_size;

    vec2 fp = fract(pix_coord);

    vec3 c00 = GAMMA_IN(tex2D(s_p, tc     - dx - dy).xyz);
    vec3 c01 = GAMMA_IN(tex2D(s_p, tc          - dy).xyz);
    vec3 c02 = GAMMA_IN(tex2D(s_p, tc     + dx - dy).xyz);
    vec3 c03 = GAMMA_IN(tex2D(s_p, tc + 2.0*dx - dy).xyz);
    vec3 c10 = GAMMA_IN(tex2D(s_p, tc     - dx).xyz);
    vec3 c11 = GAMMA_IN(tex2D(s_p, tc         ).xyz);
    vec3 c12 = GAMMA_IN(tex2D(s_p, tc     + dx).xyz);
    vec3 c13 = GAMMA_IN(tex2D(s_p, tc + 2.0*dx).xyz);

    //  Get min/max samples
    vec3 min_sample = min(min(c01,c11), min(c02,c12));
    vec3 max_sample = max(max(c01,c11), max(c02,c12));

    mat4x3 color_matrix0 = mat4x3(c00, c01, c02, c03);
    mat4x3 color_matrix1 = mat4x3(c10, c11, c12, c13);

    vec4 lobes = vec4(fp.x*fp.x*fp.x, fp.x*fp.x, fp.x, 1.0);

    vec4 invX_Px  = invX * lobes;
    vec3 color0   = color_matrix0 * invX_Px;
    vec3 color1   = color_matrix1 * invX_Px;

    // Anti-ringing
    vec3 aux = color0;
    color0 = clamp(color0, min_sample, max_sample);
    color0 = mix(aux, color0, CRT_ANTI_RINGING);
    aux = color1;
    color1 = clamp(color1, min_sample, max_sample);
    color1 = mix(aux, color1, CRT_ANTI_RINGING);

    float pos0 = fp.y;
    float pos1 = 1.0 - fp.y;

    vec3 lum0 = mix(vec3(BEAM_MIN_WIDTH), vec3(BEAM_MAX_WIDTH), color0);
    vec3 lum1 = mix(vec3(BEAM_MIN_WIDTH), vec3(BEAM_MAX_WIDTH), color1);

    vec3 d0 = clamp(pos0/(lum0+0.0000001), 0.0, 1.0);
    vec3 d1 = clamp(pos1/(lum1+0.0000001), 0.0, 1.0);

    d0 = exp(-10.0*SCANLINES_STRENGTH*d0*d0);
    d1 = exp(-10.0*SCANLINES_STRENGTH*d1*d1);

    color = clamp(color0*d0+color1*d1, 0.0, 1.0);            

    color *= COLOR_BOOST*vec3(RED_BOOST, GREEN_BOOST, BLUE_BOOST);

    float mod_factor = texCoord.x * OutputSize.x * TextureSize.x / InputSize.x;

    vec3 dotMaskWeights = mix(
                                 vec3(1.0, 0.7, 1.0),
                                 vec3(0.7, 1.0, 0.7),
                                 floor(mod(mod_factor, 2.0))
                                  );

    color.rgb *= mix(vec3(1.0,1.0,1.0), dotMaskWeights, PHOSPHOR);

    color  = GAMMA_OUT(color);

    FragColor =  vec4(color, 1.0);
}
