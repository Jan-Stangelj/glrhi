#version 460

layout(binding = 0, rgba8) uniform readonly image3D voxels;

layout (std140, binding = 2) uniform voxelSettings {
    float voxelGridSize;
    float voxelRes;
};

out vec4 voxelColor;
out vec3 voxelPos;

void main()
{
    uint size = uint(voxelRes);
    uint idx = gl_VertexID;

    uint x = idx % size;
    uint y = (idx / size) % size;
    uint z = idx / (size * size);

    ivec3 texPos = ivec3(x, y, z);
    voxelColor = imageLoad(voxels, texPos);

    voxelPos = vec3(x, y, z);
}
