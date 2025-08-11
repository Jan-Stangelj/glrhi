#version 460 core

out vec4 FragColor;

in vec2 texUV;

uniform sampler2D u_albedo;
uniform sampler2D u_normalRoughness;
uniform sampler2D u_emissionMetallic;
uniform sampler2D u_position;

void main() {
    FragColor = vec4(texture(u_position, texUV).rgb, 1.0);
}