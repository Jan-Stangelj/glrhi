#version 460

layout(binding = 0, rgba16f) uniform readonly image3D voxels;

out vec4 voxelColor;
out vec3 voxelPos;

void main()
{
    const uint size = 128;
    uint idx = gl_VertexID;

    uint x = idx % size;
    uint y = (idx / size) % size;
    uint z = idx / (size * size);

    ivec3 texPos = ivec3(x, y, z);
    voxelColor = imageLoad(voxels, texPos);

    // Raw voxel grid position [0..63]
    voxelPos = vec3(x, y, z);
}
