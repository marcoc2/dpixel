#version 400

in vec3 color;
out vec4 frag_color;

void main() {
    frag_color = vec4 (color, 1.0);
    //frag_colour = vec4 (1.0, 0.0, 0.0, 1.0);
}
