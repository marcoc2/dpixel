#version 400

in vec3 color;
out vec4 frag_color;
uniform sampler2D tex;

in vec2 uv;

void main() {
    //frag_color = vec4 (color, 1.0);
    frag_color = texture( tex, uv );
}
