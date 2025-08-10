#version 460 core

in vec2 texCoord;

out vec4 FragColor;

layout (std140, binding=1) uniform mat {
    vec4 albedo;
    vec4 arm;
    vec4 emission;

    float hasAlbedo;
    float hasARM;
    float hasNormal;
    float hasEmission;
};

uniform sampler2D u_albedo;
uniform sampler2D u_arm;
uniform sampler2D u_normal;
uniform sampler2D u_emission;

void main() 
{
    FragColor = texture(u_albedo, texCoord);
}