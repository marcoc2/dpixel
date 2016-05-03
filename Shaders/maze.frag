uniform vec3 iResolution;
uniform vec4 iMouse;
uniform float iGlobalTime;

mat2 z=mat2(1,1,1,-1);
float m(vec2 p)
{
    return step(cos(1.257*(iMouse.x>105?p.y:p.x)),.17);
}
void main()
{
    vec2 p = z * (gl_FragCoord.xy / iResolution.y * 35. + iMouse.xy*.1 + iGlobalTime*vec2(1.8,1.08)),c = floor(p);
    float s = step(1. - p.x + c.x, p.y - c.y), f = m(c), g = m(c + vec2(-1, 0) + s);
    gl_FragColor.rgb = mix(vec3(1,.3,.25),vec3(1),m(c-z[1])+(g < f ? .75 - .25 * s : .5 + .25 * s)*max(g, f));
}
