#version 120

uniform sampler2D tex;

void main()
{
    vec4 texColor = texture2D(tex,gl_TexCoord[0].st);
    //gl_FragColor = gl_Color;
    gl_FragColor = texColor;
}
