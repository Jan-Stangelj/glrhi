#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aTex;

out vec2 texCoord;

layout (std140, binding=0) uniform cam {
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    texCoord = aTex.xy;
}