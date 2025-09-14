#version 460 core

out vec4 fragColor;

in vec2 texUV;

layout(binding = 0) uniform sampler2D u_resoult;

vec3 PBRNeutralToneMapping( vec3 color ) {
  const float startCompression = 0.8 - 0.04;
  const float desaturation = 0.15;

  float x = min(color.r, min(color.g, color.b));
  float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
  color -= offset;

  float peak = max(color.r, max(color.g, color.b));
  if (peak < startCompression) return color;

  const float d = 1. - startCompression;
  float newPeak = 1. - d * d / (peak + d - startCompression);
  color *= newPeak / peak;

  float g = 1. - 1. / (desaturation * (peak - newPeak) + 1.);
  return mix(color, newPeak * vec3(1, 1, 1), g);
}

void main() {
    vec4 tex = texture(u_resoult, texUV);
    vec3 color = tex.rgb;

    // Apply tone mapping
    vec3 toneMapped = PBRNeutralToneMapping(color);

    // Gamma correction (sRGB)
    toneMapped = pow(toneMapped, vec3(1.0 / 2.2));

    fragColor = vec4(toneMapped, 1.0);
}
