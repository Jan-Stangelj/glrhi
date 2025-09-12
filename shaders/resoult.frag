#version 460 core

out vec4 fragColor;

in vec2 texUV;

uniform sampler2D u_resoult;

// ACES Filmic tone mapping function
vec3 ACESFilm(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
    vec4 tex = texture(u_resoult, texUV);
    vec3 color = tex.rgb;

    // Apply ACES tone mapping
    vec3 toneMapped = ACESFilm(color);

    // Gamma correction (sRGB)
    toneMapped = pow(toneMapped, vec3(1.0 / 2.2));

    fragColor = vec4(toneMapped, 1.0);
}
