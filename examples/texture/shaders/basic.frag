#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D txt;

void main()
{
    FragColor = pow(texture(txt, texCoord), vec4(1.0 / 2.2));
}