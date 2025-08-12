#pragma once

#include "glrhi.hpp"

#include "renderer/mesh.hpp"
#include "renderer/camera.hpp"
#include "renderer/debugcam.hpp"
#include "renderer/material.hpp"
#include "renderer/model.hpp"
#include "renderer/gbuffer.hpp"
#include "renderer/lighting.hpp"

#include "utils/timer.hpp"

#include <map>
#include <vector>
#include <string_view>
#include <filesystem>

namespace glrhi {
    class renderer {
    public:

        renderer(unsigned int width, unsigned int height, const char* title) : m_window(width, height, title), m_gBuffer(width, height) { m_gBufferShader.createFromCode(m_vertexGbufferCode.data(), m_fragmentGbufferCode.data()); m_lightingShader.create("../examples/renderer/shaders/lighting.comp"); m_width = width; m_height = height; glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE); }
        ~renderer() = default;

        glrhi::camera& getCamera() { return m_camera; }
        glrhi::window& getWindow() { return m_window; }

        void addModel(const char* name, const std::filesystem::path& model) { m_models.try_emplace(name, model); }
        void removeModel(const char* name) { m_models.erase(name); }
        glrhi::model& getModel(const char* name) { return m_models[name]; }

        void addLight(const char* name) { m_lights.try_emplace(name); }
        void removeLight(const char* name) { m_lights.erase(name); }
        glrhi::light& getLight(const char* name) { return m_lights[name]; }

        void pushPostProcessShader(const std::filesystem::path& computeShader) { m_postProcessShaders.emplace_back(computeShader); }
        void popPostProcessShader() {m_postProcessShaders.pop_back(); }

        bool running() { return !m_window.shouldClose(); }
        void getInput() { m_window.poolEvents(); m_deltaTime = m_deltaTimer.get(); m_deltaTimer.reset();}
        void gBufferPass();
        void lightingPass();
        void postProcessPass();
        void renderResoult();

        double deltaTime() { return m_deltaTime; }

    private:

        glrhi::window m_window;

        unsigned int m_width;
        unsigned int m_height;

        double m_deltaTime = 0.16;

        glrhi::gbuffer m_gBuffer;
        glrhi::shader m_gBufferShader;
        glrhi::compute m_lightingShader;

        glrhi::camera m_camera;

        glrhi::timer m_deltaTimer;

        std::map<std::string_view, glrhi::model> m_models;
        std::map<std::string_view, glrhi::light> m_lights;
        std::vector<glrhi::compute> m_postProcessShaders;

        constexpr static std::string_view m_vertexGbufferCode = "#version 460 core\n\nlayout (location = 0) in vec3 a_pos;\nlayout (location = 1) in vec2 a_texUV;\nlayout (location = 2) in vec3 a_normal;\nlayout (location = 3) in vec3 a_tangent;\n\nout vec2 texUV;\nout vec3 fragPos;\nout mat3 TBN;\n\nlayout (std140, binding=0) uniform cam {\n    mat4 view;\n    mat4 projection;\n};\n\nuniform mat4 u_model;\n\nvoid main() {\n    gl_Position = projection * view * u_model * vec4(a_pos, 1.0);\n\n    fragPos = vec3(u_model * vec4(a_pos, 1.0));\n\n    texUV = vec2(a_texUV.x, a_texUV.y);\n\n    vec3 T = normalize(vec3(u_model * vec4(a_tangent, 0.0)));\n\tvec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));\n\tvec3 B = cross(T, N);\n\tTBN = mat3(T, B, N);\n\n}";
        constexpr static std::string_view m_fragmentGbufferCode = "#version 460 core\n\nlayout(location = 0) out vec3 g_albedo;\nlayout(location = 1) out vec4 g_normalRoughness;\nlayout(location = 2) out vec4 g_emissionMetallic;\nlayout(location = 3) out vec3 g_position;\nlayout(location = 4) out vec3 g_resoult;\n\nin vec2 texUV;\nin vec3 fragPos;\nin mat3 TBN;\n\nlayout (std140, binding=1) uniform mat {\n    vec4 albedo;\n    vec4 arm;\n    vec4 emission;\n\n    float hasAlbedo;\n    float hasARM;\n    float hasNormal;\n    float hasEmission;\n};\n\nuniform sampler2D u_albedo;\nuniform sampler2D u_arm;\nuniform sampler2D u_normal;\nuniform sampler2D u_emission;\n\nvoid main() \n{\n    if (texture(u_albedo, texUV).a == 0)\n        discard;\n\n    vec3 normal = texture(u_normal, texUV).rgb;\n\tnormal = normal * 2.0 - 1.0;\n\tnormal = normalize(TBN * normal);\n\n    vec4 albedoOut = texture(u_albedo, texUV) * hasAlbedo + albedo * (1-hasAlbedo);\n    vec4 armOut = texture(u_arm, texUV) * hasARM + arm * (1-hasARM);\n    vec4 emissionOut = texture(u_emission, texUV) * hasEmission + emission * (1-hasARM);\n\n    g_albedo = albedoOut.xyz;\n    g_normalRoughness = vec4(normal, arm.g);\n    g_emissionMetallic = vec4(emissionOut.xyz, arm.b);\n    g_position = fragPos;\n    g_resoult = albedoOut.xyz + g_emissionMetallic.xyz;\n}";
    };
}