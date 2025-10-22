#version 460 core

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

layout (std430, binding = 0) buffer voxelOut {
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
        ((uintVec.g << uint(0)) & uint(0x000000FF)));
}

void main() 
{
    if (texture(u_albedo, texUVout).a == 0)
        discard;

    vec4 albedoOut = texture(u_albedo, texUVout) * hasAlbedo + albedo * (1-hasAlbedo);
    albedoOut = vec4(albedoOut.rgb, 1.0);
    ivec3 voxelCoord = ivec3(floor(clamp((voxelPosout + vec3(voxelGridSize / 2))/voxelGridSize, 0.0f, 1.0f) * voxelRes));
    voxelCoord = clamp(voxelCoord, 0, int(voxelRes - 1));

    uint voxelCoord1D = uint(voxelCoord.x) + uint(voxelCoord.y) * uint(voxelRes) + uint(voxelCoord.z) * uint(voxelRes) * uint(voxelRes);

    voxels[voxelCoord1D] = vec4ToUint(albedoOut);

    uint newVal = vec4ToUint(albedoOut);
    uint prevStoredVal = 0;
    uint curStoredVal;
    uint numIterations = 0;

    while((curStoredVal = atomicCompSwap(voxels[voxelCoord1D], prevStoredVal, newVal)) 
            != prevStoredVal
            && numIterations < 255)
    {
        prevStoredVal = curStoredVal;
        vec4 rval = uintToVec4(curStoredVal);
        rval.rgb = (rval.rgb * rval.a); // Denormalize
        vec4 curValF = rval + albedoOut;    // Add
        curValF.rgb /= curValF.a;       // Renormalize
        newVal = vec4ToUint(curValF);

        ++numIterations;
    }
}