#pragma once

#include <string_view>

namespace glrhi {
    constexpr static std::string_view vertexGbufferCode = "#version 460 core\n"
                                                            "\n"
                                                            "layout (location = 0) in vec3 a_pos;\n"
                                                            "layout (location = 1) in vec2 a_texUV;\n"
                                                            "layout (location = 2) in vec3 a_normal;\n"
                                                            "layout (location = 3) in vec3 a_tangent;\n"
                                                            "\n"
                                                            "out vec2 texUV;\n"
                                                            "out vec3 fragPos;\n"
                                                            "out mat3 TBN;\n"
                                                            "\n"
                                                            "layout (std140, binding=0) uniform cam {\n"
                                                            "    mat4 view;\n"
                                                            "    mat4 projection;\n"
                                                            "};\n"
                                                            "\n"
                                                            "uniform mat4 u_model;\n"
                                                            "\n"
                                                            "void main() {\n"
                                                            "    gl_Position = projection * view * u_model * vec4(a_pos, 1.0);\n"
                                                            "\n"
                                                            "    fragPos = vec3(u_model * vec4(a_pos, 1.0));\n"
                                                            "\n"
                                                            "    texUV = vec2(a_texUV.x, a_texUV.y);\n"
                                                            "\n"
                                                            "    vec3 T = normalize(vec3(u_model * vec4(a_tangent, 0.0)));\n"
                                                            "\tvec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));\n"
                                                            "\tvec3 B = cross(T, N);\n"
                                                            "\tTBN = mat3(T, B, N);\n"
                                                            "\n"
                                                            "}";

        constexpr static std::string_view fragmentGbufferCode = "#version 460 core\n"
                                                                    "\n"
                                                                    "layout(location = 0) out vec3 g_albedo;\n"
                                                                    "layout(location = 1) out vec4 g_normalRoughness;\n"
                                                                    "layout(location = 2) out vec4 g_emissionMetallic;\n"
                                                                    "layout(location = 3) out vec3 g_position;\n"
                                                                    "layout(location = 4) out vec3 g_resoult;\n"
                                                                    "\n"
                                                                    "in vec2 texUV;\n"
                                                                    "in vec3 fragPos;\n"
                                                                    "in mat3 TBN;\n"
                                                                    "\n"
                                                                    "layout (std140, binding=1) uniform mat {\n"
                                                                    "    vec4 albedo;\n"
                                                                    "    vec4 arm;\n"
                                                                    "    vec4 emission;\n"
                                                                    "\n"
                                                                    "    float hasAlbedo;\n"
                                                                    "    float hasARM;\n"
                                                                    "    float hasNormal;\n"
                                                                    "    float hasEmission;\n"
                                                                    "};\n"
                                                                    "\n"
                                                                    "uniform sampler2D u_albedo;\n"
                                                                    "uniform sampler2D u_arm;\n"
                                                                    "uniform sampler2D u_normal;\n"
                                                                    "uniform sampler2D u_emission;\n"
                                                                    "\n"
                                                                    "void main() \n"
                                                                    "{\n"
                                                                    "    if (texture(u_albedo, texUV).a == 0)\n"
                                                                    "        discard;\n"
                                                                    "\n"
                                                                    "    vec3 normal = texture(u_normal, texUV).rgb;\n"
                                                                    "\tnormal = normal * 2.0 - 1.0;\n"
                                                                    "\tnormal = normalize(TBN * normal);\n"
                                                                    "\n"
                                                                    "    vec4 albedoOut = texture(u_albedo, texUV) * hasAlbedo + albedo * (1-hasAlbedo);\n"
                                                                    "    vec4 armOut = texture(u_arm, texUV) * hasARM + arm * (1-hasARM);\n"
                                                                    "    vec4 emissionOut = texture(u_emission, texUV) * hasEmission + emission * (1-hasARM);\n"
                                                                    "\n"
                                                                    "    g_albedo = albedoOut.xyz;\n"
                                                                    "    g_normalRoughness = vec4(normal, arm.g);\n"
                                                                    "    g_emissionMetallic = vec4(emissionOut.xyz, arm.b);\n"
                                                                    "    g_position = fragPos;\n"
                                                                    "    g_resoult = albedoOut.xyz + g_emissionMetallic.xyz;\n"
                                                                    "}";

    constexpr static std::string_view lightingShaderCode = "#version 460 core\n"
                                                            "\n"
                                                            "layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;\n"
                                                            "\n"
                                                            "struct light {\n"
                                                            "    vec4 position;\n"
                                                            "    vec4 direction;\n"
                                                            "    vec4 color;\n"
                                                            "    float strenght;\n"
                                                            "    int type;\n"
                                                            "};\n"
                                                            "\n"
                                                            "const float PI = 3.14159265359;\n"
                                                            "\n"
                                                            "layout (std140, binding=0) uniform cam {\n"
                                                            "    mat4 view;\n"
                                                            "    mat4 projection;\n"
                                                            "    vec4 camPos;\n"
                                                            "};\n"
                                                            "\n"
                                                            "layout (std430, binding = 0) readonly buffer lights {\n"
                                                            "    light inLights[1024];\n"
                                                            "};\n"
                                                            "uniform int u_numLights;\n"
                                                            "\n"
                                                            "layout(rgba16f, binding = 0) uniform image2D u_resoult;\n"
                                                            "layout(rgba16f, binding = 1) uniform image2D u_albedoInLightingOut;\n"
                                                            "\n"
                                                            "uniform sampler2D u_normalRoughness;\n"
                                                            "uniform sampler2D u_emissionMetallic;\n"
                                                            "uniform sampler2D u_position;\n"
                                                            "uniform sampler2D u_skybox;\n"
                                                            "\n"
                                                            "vec3 calcLight(vec3 albedo, float roughness, float metallic, vec3 normal, vec3 radiance, vec3 lightDir, vec3 fragPos);\n"
                                                            "\n"
                                                            "vec3 fresnelSchlick(float cosTheta, vec3 F0);\n"
                                                            "\n"
                                                            "vec2 sampleEquirectangular(vec3 dir) {\n"
                                                            "    float phi = atan(dir.z, dir.x);\n"
                                                            "    float theta = acos(clamp(dir.y, -1.0, 1.0));\n"
                                                            "\n"
                                                            "    float u = phi / (2.0 * PI) + 0.5;\n"
                                                            "    float v = theta / PI;\n"
                                                            "    return vec2(u, v);\n"
                                                            "}\n"
                                                            "\n"
                                                            "vec3 sampleIBL(vec3 reflDir, float roughness) {\n"
                                                            "    vec2 uv = sampleEquirectangular(normalize(reflDir));\n"
                                                            "    float maxMip = float(textureQueryLevels(u_skybox) - 1);\n"
                                                            "    float mipCount = float(textureQueryLevels(u_skybox));\n"
                                                            "    float mip = pow(roughness, roughness) * 6; // more perceptual\n"
                                                            "    mip = clamp(mip, 0.0, mipCount - 1.0);\n"
                                                            "    return textureLod(u_skybox, uv, mip).rgb;\n"
                                                            "}\n"
                                                            "\n"
                                                            "\n"
                                                            "void main() {\n"
                                                            "    vec2 texUV = vec2(0.0);\n"
                                                            "    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);\n"
                                                            "    texUV.x = float(texelCoord.x)/(gl_NumWorkGroups.x);\n"
                                                            "    texUV.y = float(texelCoord.y)/(gl_NumWorkGroups.y);\n"
                                                            "\n"
                                                            "    vec4 normalRoughness = texture(u_normalRoughness, texUV);\n"
                                                            "    vec4 emissionMetallic = texture(u_emissionMetallic, texUV);\n"
                                                            "\n"
                                                            "    vec3 albedo = pow(imageLoad(u_albedoInLightingOut, texelCoord).rgb, vec3(2.2));\n"
                                                            "    vec3 arm = vec3(1.0, normalRoughness.w, emissionMetallic.w);\n"
                                                            "    vec3 normal = normalRoughness.rgb;\n"
                                                            "    vec3 position = texture(u_position, texUV).rgb;\n"
                                                            "\n"
                                                            "    vec3 resoult = vec3(0.0);\n"
                                                            "\n"
                                                            "    light l = inLights[0];\n"
                                                            "\n"
                                                            "    //resoult += calcLight(albedo, arm.g, arm.b, normal, l.color.xyz * l.strenght, l.direction.xyz, position);\n"
                                                            "\n"
                                                            "\tfor (int i = 0; i < u_numLights; i++) {\n"
                                                            "\t\tlight l = inLights[i];\n"
                                                            "\n"
                                                            "        if (l.type == 0)\n"
                                                            "            resoult += calcLight(albedo, arm.g, arm.b, normal, l.color.xyz * l.strenght, l.direction.xyz, position);\n"
                                                            "\n"
                                                            "\t\tvec3 lightDir = l.position.xyz - position;\n"
                                                            "\t\tfloat distance = length(lightDir);\n"
                                                            "\t\tfloat attenuation = l.strenght / pow(distance, distance);\n"
                                                            "\n"
                                                            "        if (l.type == 1)\n"
                                                            "            resoult += calcLight(albedo, arm.g, arm.b, normal, l.color.xyz * attenuation, lightDir, position);\n"
                                                            "\t\t\n"
                                                            "\t\tfloat theta = dot(normalize(lightDir), normalize(-l.direction.xyz));\n"
                                                            "\t}\n"
                                                            "\n"
                                                            "    vec3 V = normalize(camPos.xyz - position);\n"
                                                            "    vec3 N = normalize(normal);\n"
                                                            "    vec3 R = reflect(-V, N);\n"
                                                            "\n"
                                                            "    vec3 F0 = mix(vec3(0.04), albedo, arm.b); // arm.b = metallic\n"
                                                            "    vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0);\n"
                                                            "\n"
                                                            "    // IBL specular reflection\n"
                                                            "    vec3 iblSpec = sampleIBL(R, arm.g); // arm.g = roughness\n"
                                                            "    vec3 kS = F;\n"
                                                            "    vec3 kD = (1.0 - kS) * (1.0 - arm.b);\n"
                                                            "\n"
                                                            "    // Optional: crude IBL diffuse = last mip with normal direction\n"
                                                            "    vec3 iblDiffuse = textureLod(u_skybox, sampleEquirectangular(N), float(textureQueryLevels(u_skybox) - 1)).rgb;\n"
                                                            "\n"
                                                            "    // Combine\n"
                                                            "    vec3 ibl = (kD * albedo * iblDiffuse + iblSpec * F);\n"
                                                            "\n"
                                                            "    // Combine with direct lighting\n"
                                                            "    resoult += ibl;\n"
                                                            "\n"
                                                            "    resoult += emissionMetallic.xyz;\n"
                                                            "\n"
                                                            "\n"
                                                            "    if (position != vec3(0.0)) {\n"
                                                            "        imageStore(u_resoult, texelCoord, vec4(resoult, 1.0));\n"
                                                            "    }\n"
                                                            "\n"
                                                            "}\n"
                                                            "\n"
                                                            "// --------------------------------------------------------------------------------------------------------------------------\n"
                                                            "// --------------------------------------------------------------------------------------------------------------------------\n"
                                                            "\n"
                                                            "float DistributionGGX(vec3 N, vec3 H, float roughness) {\n"
                                                            "    float a = roughness*roughness;\n"
                                                            "    float a2 = a*a;\n"
                                                            "    float NdotH = max(dot(N, H), 0.0);\n"
                                                            "    float NdotH2 = NdotH*NdotH;\n"
                                                            "\n"
                                                            "    float nom   = a2;\n"
                                                            "    float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
                                                            "    denom = PI * denom * denom;\n"
                                                            "\n"
                                                            "    return nom / denom;\n"
                                                            "}\n"
                                                            "float GeometrySchlickGGX(float NdotV, float roughness) {\n"
                                                            "    float r = (roughness + 1.0);\n"
                                                            "    float k = (r*r) / 8.0;\n"
                                                            "\n"
                                                            "    float nom   = NdotV;\n"
                                                            "    float denom = NdotV * (1.0 - k) + k;\n"
                                                            "\n"
                                                            "    return nom / denom;\n"
                                                            "}\n"
                                                            "float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {\n"
                                                            "    float NdotV = max(dot(N, V), 0.0);\n"
                                                            "    float NdotL = max(dot(N, L), 0.0);\n"
                                                            "    float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
                                                            "    float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"
                                                            "\n"
                                                            "    return ggx1 * ggx2;\n"
                                                            "}\n"
                                                            "vec3 fresnelSchlick(float cosTheta, vec3 F0) {\n"
                                                            "    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
                                                            "}\n"
                                                            "\n"
                                                            "vec3 calcLight(vec3 albedo, float roughness, float metallic, vec3 normal, vec3 radiance, vec3 lightDir, vec3 fragPos) {\n"
                                                            "\n"
                                                            "    vec3 N = normal;\n"
                                                            "    vec3 V = normalize(camPos.xyz - fragPos);\n"
                                                            "\n"
                                                            "\tvec3 F0 = vec3(0.04);\n"
                                                            "    F0 = mix(F0, albedo, metallic);\n"
                                                            "\n"
                                                            "\t// calculate per-light radiance\n"
                                                            "\tvec3 L = normalize(lightDir);\n"
                                                            "\tvec3 H = normalize(V + L);\n"
                                                            "\n"
                                                            "\t// Cook-Torrance BRDF\n"
                                                            "\tfloat NDF = DistributionGGX(N, H, roughness);\n"
                                                            "\tfloat G   = GeometrySmith(N, V, L, roughness);\n"
                                                            "\tvec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);\n"
                                                            "\t\t\n"
                                                            "\tvec3 numerator    = NDF * G * F;\n"
                                                            "\tfloat denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero\n"
                                                            "\tvec3 specular = numerator / denominator;\n"
                                                            "\t\n"
                                                            "\t// kS is equal to Fresnel\n"
                                                            "\tvec3 kS = F;\n"
                                                            "\tvec3 kD = vec3(1.0) - kS;\n"
                                                            "\n"
                                                            "\tkD *= 1.0 - metallic;\n"
                                                            "\n"
                                                            "\t// scale light by NdotL\n"
                                                            "\tfloat NdotL = max(dot(N, L), 0.0);  \n"
                                                            "\n"
                                                            "\treturn (kD * albedo / PI + specular) * radiance * NdotL;\n"
                                                            "}";

    constexpr float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    constexpr unsigned int skyboxIndices[] {
        0, 1, 2,
        3, 4, 5,
        
        6, 7, 8,
        9, 10, 11,

        12, 13, 14,
        15, 16, 17,

        18, 19, 20,
        21, 22, 23,

        24, 25, 26,
        27, 28, 29,

        30, 31, 32,
        33, 34, 35
    };

    constexpr std::string_view skyboxVert = "#version 460 core\n"
                                "\n"
                                "layout(location = 0) in vec3 a_pos;\n"
                                "\n"
                                "layout(std140, binding=0) uniform cam {\n"
                                "    mat4 view;\n"
                                "    mat4 projection;\n"
                                "};\n"
                                "\n"
                                "out vec3 texCoord;\n"
                                "\n"
                                "void main() {\n"
                                "    gl_Position = (projection * mat4(mat3(view)) * vec4(a_pos, 1.0)).xyww;\n"
                                "    texCoord = a_pos;\n"
                                "}";

    constexpr std::string_view skyboxFrag = "#version 460 core\n"
                                "\n"
                                "layout(location = 4) out vec3 g_resoult;\n"
                                "\n"
                                "in vec3 texCoord;\n"
                                "\n"
                                "uniform sampler2D u_skybox;\n"
                                "\n"
                                "// Convert normalized direction vector to equirectangular UV\n"
                                "vec2 sampleSphericalMap(vec3 v)\n"
                                "{\n"
                                "    vec3 dir = normalize(v);\n"
                                "\n"
                                "    float u = atan(dir.z, dir.x) / (2.0 * 3.14159265) + 0.5;\n"
                                "    float v_ = asin(dir.y) / 3.14159265 + 0.5;\n"
                                "\n"
                                "    return vec2(u, v_);\n"
                                "}\n"
                                "\n"
                                "void main() {\n"
                                "    vec2 uv = sampleSphericalMap(texCoord);\n"
                                "    g_resoult = textureLod(u_skybox, uv, 0).rgb;\n"
                                "}\n";
}