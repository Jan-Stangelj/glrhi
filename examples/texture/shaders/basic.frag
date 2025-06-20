#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D txt;

void main()
{
    vec3 txtColor = pow(texture(txt, texCoord).xyz, vec3(2.2));

    FragColor = pow(vec4(txtColor, 1.0), vec4(1.0 / 2.2));
}