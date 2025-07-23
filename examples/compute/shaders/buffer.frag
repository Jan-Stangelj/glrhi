#version 460 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screen;

void main()
{
    FragColor = texture(screen, texCoord);
}