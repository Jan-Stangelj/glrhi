#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 texCoord;

layout (std140, binding=0) uniform cam {
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    texCoord = vec2(aTex.x, aTex.y);
}