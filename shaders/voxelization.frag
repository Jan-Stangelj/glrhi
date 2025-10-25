#version 460 core

in vec2 texUVout;
in vec3 voxelPosout;
in vec3 normalOut;

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

layout (std430, binding = 1) buffer voxelOut {
    uint[] voxels;
};

uniform sampler2D u_albedo;
uniform sampler2D u_arm;
uniform sampler2D u_normal;
uniform sampler2D u_emission;

vec4 uintToVec4(uint x) {
    float r = float((x >> uint(24)) & uint(0x000000FF)) / 255.0;
    float g = float((x >> uint(16)) & uint(0x000000FF)) / 255.0;
    float b = float((x >> uint(8)) & uint(0x000000FF)) / 255.0;
    float a = float((x >> uint(0)) & uint(0x000000FF)) / 255.0;
    return vec4(r, g, b, a);
}

uint vec4ToUint(vec4 x) {
    uvec4 uintVec = uvec4(floor(x * 255.0));
    return uint(
        ((uintVec.r << uint(24)) & uint(0xFF000000)) | 
        ((uintVec.g << uint(16)) & uint(0x00FF0000)) | 
        ((uintVec.b << uint(8)) & uint(0x0000FF00)) | 
        ((uintVec.a << uint(0)) & uint(0x000000FF)));
}

void main() 
{
    if (texture(u_albedo, texUVout).a == 0)
        discard;

    vec4 albedoOut = texture(u_albedo, texUVout) * hasAlbedo + albedo * (1-hasAlbedo);

    ivec3 voxelCoord = ivec3(floor(clamp((voxelPosout + vec3(voxelGridSize / 2))/voxelGridSize, 0.0f, 1.0f) * voxelRes));
    voxelCoord = clamp(voxelCoord, 0, int(voxelRes - 1));

    uint voxelCoord1D = uint(voxelCoord.x) + uint(voxelCoord.y) * uint(voxelRes) + uint(voxelCoord.z) * uint(voxelRes) * uint(voxelRes);
    voxelCoord1D *= 2;


    uint newAlbedo = vec4ToUint(albedoOut);
    uint newNormal = vec4ToUint(clamp(vec4(normalOut + vec3(1) * 0.5, 1.0), 0, 1));

    uint previousAlbedo = 0;
    uint previousNormal = 0;

    uint currentAlbedo, currentNormal;
    uint num = 0;


    while(num < 4)
    {
        currentAlbedo = atomicCompSwap(voxels[voxelCoord1D + 0], previousAlbedo, newAlbedo);
        currentNormal = atomicCompSwap(voxels[voxelCoord1D + 1], previousNormal, newNormal);

        if(currentAlbedo == previousAlbedo && currentNormal == previousNormal)
            break;

        previousAlbedo = currentAlbedo;
        previousNormal = currentNormal;

        vec4 albedoF = uintToVec4(currentAlbedo);
        albedoF.rgb *= albedoF.a;
        vec4 normalF = uintToVec4(currentNormal);


        vec4 avgAlbedo = (albedoF + albedoOut) * 0.5;
        vec4 avgNormal = (normalF + vec4(normalOut, 1.0)) * 0.5;

        newAlbedo = vec4ToUint(avgAlbedo);
        newNormal = vec4ToUint(avgNormal);

        ++num;
    }

}