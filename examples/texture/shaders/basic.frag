#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D txt;

void main()
{
    FragColor = texture(txt, texCoord);
}