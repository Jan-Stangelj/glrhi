#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screen;

void main()
{
    FragColor = texture(screen, texCoord);
}