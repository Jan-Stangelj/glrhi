#version 460

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec4 voxelColor[];
in vec3 voxelPos[];

out vec4 fragColor;

layout(std140, binding = 0) uniform cam {
    mat4 view;
    mat4 projection;
    vec4 camPos;
};

vec3 VoxelToWorld(vec3 pos) {
    return pos * (40.0 / 128.0) - vec3(20.0);
}

void main()
{
    const vec3 cubeVertices[8] = vec3[8](
        vec3( 0.5,  0.5,  0.5),
        vec3( 0.5,  0.5, -0.5),
        vec3( 0.5, -0.5,  0.5),
        vec3( 0.5, -0.5, -0.5),
        vec3(-0.5,  0.5,  0.5),
        vec3(-0.5,  0.5, -0.5),
        vec3(-0.5, -0.5,  0.5),
        vec3(-0.5, -0.5, -0.5)
    );

    const int cubeIndices[24] = int[24](
        0, 2, 1, 3, // right
        6, 4, 7, 5, // left
        5, 4, 1, 0, // up
        6, 7, 2, 3, // down
        4, 6, 0, 2, // front
        1, 3, 5, 7  // back
    );

    vec3 center = VoxelToWorld(voxelPos[0]);
    vec3 extent = vec3(40.0 / 128.0);

    vec4 projectedVertices[8];
    for (int i = 0; i < 8; ++i) {
        vec3 worldPos = center + extent * cubeVertices[i];
        projectedVertices[i] = projection * view * vec4(worldPos, 1.0);
    }

    for (int face = 0; face < 6; ++face) {
        for (int v = 0; v < 4; ++v) {
            gl_Position = projectedVertices[cubeIndices[face * 4 + v]];
            fragColor = voxelColor[0];
            EmitVertex();
        }
        EndPrimitive();
    }
}
