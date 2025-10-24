#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 2) uniform voxelSettings {
    float voxelGridSize;
    float voxelRes;
};

in vec3 pos[];
in vec3 normal[];
in vec2 texUV[];
out vec3 normalOut;
out vec2 texUVout;
out vec3 voxelPosout; // world coordinates scaled to clip space (-1...1)

void main()
{
	//
	// we're projecting the triangle face orthogonally with the dominant axis of its normal vector.
	// the end goal is to maximize the amount of generated fragments. more details at OpenGL Insights, pages 303-318
	//

	mat3 swizzle_mat;

	const vec3 edge1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	const vec3 edge2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	const vec3 face_normal = abs(cross(edge1, edge2)); 

	if (face_normal.x >= face_normal.y && face_normal.x >= face_normal.z) { // see: Introduction to Geometric Computing, page 33 (Ghali, 2008)
		swizzle_mat = mat3(
			vec3(0.0, 0.0, 1.0),
			vec3(0.0, 1.0, 0.0),
			vec3(1.0, 0.0, 0.0));
	} else if (face_normal.y >= face_normal.z) {
		swizzle_mat = mat3(
			vec3(1.0, 0.0, 0.0),
			vec3(0.0, 0.0, 1.0),
			vec3(0.0, 1.0, 0.0));
	} else {
		swizzle_mat = mat3(
			vec3(1.0, 0.0, 0.0),
			vec3(0.0, 1.0, 0.0),
			vec3(0.0, 0.0, 1.0));
	}

	vec3 trigCenter = vec3(gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz) / 3;
	float halfAPixel = (voxelGridSize / voxelRes) / 2;

	for (int i=0; i < 3; i++)
	{
		vec3 transformedPos = gl_in[i].gl_Position.xyz * swizzle_mat;
		vec3 outVec = normalize(gl_in[i].gl_Position.xyz - trigCenter);
		transformedPos += outVec * halfAPixel;
		gl_Position = vec4(transformedPos, 1.0);

		voxelPosout = pos[i];	
		normalOut = normal[i];
		texUVout = texUV[i];

		EmitVertex();
	}

	EndPrimitive();
}