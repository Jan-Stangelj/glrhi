#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screen;

void main()
{
    FragColor = vec4(texture(screen, texCoord).rgb, 1.0f);
}