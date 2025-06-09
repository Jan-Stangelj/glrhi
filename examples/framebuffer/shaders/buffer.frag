#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screen;

void main()
{
    FragColor = vec4(vec3(1.0) - texture(screen, texCoord).rgb, 1.0f);
}