#version 460

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec4 voxelColor[];
in vec3 voxelPos[];

out vec4 fragColor;

layout(std140, binding = 0) uniform cam {
    mat4 view;
    mat4 projection;
};

layout (std140, binding = 2) uniform voxelSettings {
    float voxelGridSize;
    float voxelRes;
};

vec3 voxelToWorld(vec3 pos) {
    return pos * (voxelGridSize / voxelRes) - vec3(voxelGridSize * 0.5);
}

void main()
{
    const vec3 verts[8] = vec3[8](
        vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5, -0.5),
        vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5, -0.5),
        vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5, -0.5),
        vec3(-0.5, -0.5,  0.5), vec3(-0.5, -0.5, -0.5)
    );
    const int faces[24] = int[24](
        0,2,1,3,  6,4,7,5,  5,4,1,0,
        6,7,2,3,  4,6,0,2,  1,3,5,7
    );

    vec3 center = voxelToWorld(voxelPos[0]);
    vec3 halfSize = vec3(voxelGridSize / voxelRes);

    vec4 proj[8];
    for (int i = 0; i < 8; ++i) {
        vec3 wp = center + halfSize * verts[i];
        proj[i] = projection * view * vec4(wp, 1.0);
    }

    for (int i = 0; i < 24; i += 4) {
        for (int j = 0; j < 4; ++j) {
            gl_Position = proj[faces[i + j]];
            fragColor = voxelColor[0];
            EmitVertex();
        }
        EndPrimitive();
    }
}
