#version 460 core

layout(binding = 0, rgba16f) uniform image3D voxelOut;

in vec2 texUV;
in vec3 pos;
in mat3 TBN;

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
    if (texture(u_albedo, texUV).a == 0)
        discard;

    vec4 albedoOut = texture(u_albedo, texUV) * hasAlbedo + albedo * (1-hasAlbedo);
    ivec3 voxelCoord = ivec3(floor(clamp((pos + vec3(20.0))/40.0, 0.0f, 1.0f) * 128.0));
    voxelCoord = clamp(voxelCoord, 0, 127);
    imageStore(voxelOut, voxelCoord, albedoOut);

}