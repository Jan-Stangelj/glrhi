#version 460 core

layout (location = 0) out vec4 gAlbedo;

uniform vec3 color;

void main()
{
    gAlbedo = vec4(color, 1.0f);
}