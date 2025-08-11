#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texUV;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_tangent;

out vec2 texUV;
out vec3 fragPos;
out mat3 TBN;

layout (std140, binding=0) uniform cam {
    mat4 view;
    mat4 projection;
};

uniform mat4 u_model;

void main() {
    gl_Position = projection * view * u_model * vec4(a_pos, 1.0);

    fragPos = vec3(u_model * vec4(a_pos, 1.0));

    texUV = vec2(a_texUV.x, a_texUV.y);

    vec3 T = normalize(vec3(u_model * vec4(a_tangent, 0.0)));
	vec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));
	vec3 B = cross(T, N);
	TBN = mat3(T, B, N);

}