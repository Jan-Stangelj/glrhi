#version 460 core

layout (std140, binding = 0) uniform ColorExample {
    vec4 color;
};

out vec4 FragColor;

void main()
{
    FragColor = color;
}