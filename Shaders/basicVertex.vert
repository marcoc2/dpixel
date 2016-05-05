#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 color;
out vec2 uv;

void main() {
        color = vertex_color;
        gl_Position = vec4(vertex_position, 1.0);
        uv = vertex_uv;
}
