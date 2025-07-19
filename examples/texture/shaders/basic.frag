#version 460 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec2 texCoord;

layout(bindless_sampler) uniform sampler2D txt;

void main()
{
    vec3 txtColor = pow(texture(txt, texCoord).xyz, vec3(2.2));

    FragColor = pow(vec4(txtColor, 1.0), vec4(1.0 / 2.2));
}