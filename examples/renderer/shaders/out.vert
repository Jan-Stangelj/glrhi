#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texUV;

out vec2 texUV;

void main() {
    gl_Position = vec4(a_pos, 1.0);
    texUV = a_texUV;
}