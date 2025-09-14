#version 460 core

layout(location = 4) out vec3 g_resoult;

in vec3 texCoord;

layout(binding = 0) uniform sampler2D u_skybox;

// Convert normalized direction vector to equirectangular UV
vec2 sampleSphericalMap(vec3 v)
{
    vec3 dir = normalize(v);

    float u = atan(dir.z, dir.x) / (2.0 * 3.14159265) + 0.5;
    float v_ = asin(dir.y) / 3.14159265 + 0.5;

    return vec2(u, v_);
}

void main() {
    vec2 uv = sampleSphericalMap(texCoord);
    g_resoult = textureLod(u_skybox, uv, 0).rgb;
}
