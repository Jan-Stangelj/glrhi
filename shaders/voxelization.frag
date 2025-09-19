#version 460 core

layout(binding = 0, rgba16f) uniform image3D voxelOut;

in vec2 texUVout;
in vec3 voxelPosout;
in mat3 TBNout;

layout (std140, binding=1) uniform mat {
    vec4 albedo;
    vec4 arm;
    vec4 emission;

    float hasAlbedo;
    float hasARM;
    float hasNormal;
    float hasEmission;
};

layout (std140, binding = 2) uniform voxelSettings {
    float voxelGridSize;
    float voxelRes;
};

uniform sampler2D u_albedo;
uniform sampler2D u_arm;
uniform sampler2D u_normal;
uniform sampler2D u_emission;

void main() 
{
    if (texture(u_albedo, texUVout).a == 0)
        discard;

    vec4 albedoOut = texture(u_albedo, texUVout) * hasAlbedo + albedo * (1-hasAlbedo);
    ivec3 voxelCoord = ivec3(floor(clamp((voxelPosout + vec3(voxelGridSize / 2))/voxelGridSize, 0.0f, 1.0f) * voxelRes));
    voxelCoord = clamp(voxelCoord, 0, int(voxelRes - 1));
    imageStore(voxelOut, voxelCoord, albedoOut);

}