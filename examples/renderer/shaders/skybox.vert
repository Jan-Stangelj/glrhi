#version 460 core

layout(location = 0) in vec3 a_pos;

layout(std140, binding=0) uniform cam {
    mat4 view;
    mat4 projection;
};

out vec3 texCoord;

void main() {
    gl_Position = (projection * mat4(mat3(view)) * vec4(a_pos, 1.0)).xyww;
    texCoord = a_pos;
}