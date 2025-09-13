#version 460 core

layout(location = 0) out vec3 g_albedo;
layout(location = 1) out vec4 g_normalRoughness;
layout(location = 2) out vec4 g_emissionMetallic;
layout(location = 3) out vec3 g_position;
layout(location = 4) out vec3 g_resoult;

in vec2 texUV;
in vec3 fragPos;
in mat3 TBN;

layout (std140, binding=0) uniform cam {
    mat4 view;
    mat4 projection;
    vec4 camPos;
};

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

layout(binding = 8) uniform sampler3D voxels;

void main() 
{
    if (texture(u_albedo, texUV).a == 0)
        discard;

    vec3 normal = texture(u_normal, texUV).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(TBN * normal);

    vec4 albedoOut = texture(u_albedo, texUV) * hasAlbedo + albedo * (1-hasAlbedo);
    vec4 armOut = texture(u_arm, texUV) * hasARM + arm * (1-hasARM);
    vec4 emissionOut = texture(u_emission, texUV) * hasEmission + emission * (1-hasARM);

    g_albedo = albedoOut.xyz;
    
    vec3 voxelCoord = (fragPos + vec3(20.0)) / 40.0;
    voxelCoord = clamp(voxelCoord, 0.0, 1.0); // optional, prevents sampling border
    g_albedo = texture(voxels, voxelCoord).rgb;

    g_normalRoughness = vec4(normal, armOut.g);
    g_emissionMetallic = vec4(emissionOut.xyz, armOut.b);
    g_position = fragPos;
    g_resoult = albedoOut.xyz + g_emissionMetallic.xyz;
}