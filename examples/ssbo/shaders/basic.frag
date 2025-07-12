#version 460 core

out vec4 FragColor;

layout (std430, binding = 0) readonly buffer colorExample {
    vec4 color;
};

void main()
{
    FragColor = color;
}