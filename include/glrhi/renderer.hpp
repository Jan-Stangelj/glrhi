#pragma once

#include "glrhi.hpp"

#include "glrhi/core/ebo.hpp"
#include "glrhi/core/vao.hpp"
#include "glrhi/core/vbo.hpp"
#include "renderer/camera.hpp"
#include "renderer/debugcam.hpp"
#include "renderer/gbuffer.hpp"
#include "renderer/scene.hpp"

#include "utils/timer.hpp"

#include <vector>
#include <filesystem>

namespace glrhi {
    class renderer {
    public:

        renderer(unsigned int width, unsigned int height, const char* title);
        ~renderer() = default;

        glrhi::camera& getCamera() { return m_camera; }
        glrhi::window& getWindow() { return m_window; }

        void pushPostProcessShader(const std::filesystem::path& computeShader) { m_postProcessShaders.emplace_back(computeShader); }
        void popPostProcessShader() {m_postProcessShaders.pop_back(); }

        bool running() { return !m_window.shouldClose(); }
        void getInput() { m_window.poolEvents(); m_deltaTime = m_deltaTimer.get(); m_deltaTimer.reset();}
        void gBufferPass(const glrhi::scene& scene);
        void lightingPass(const glrhi::scene& scene);
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
        glrhi::shader m_skyboxShader;
        glrhi::compute m_lightingShader;

        glrhi::camera m_camera;

        glrhi::timer m_deltaTimer;

        std::vector<glrhi::compute> m_postProcessShaders;

        glrhi::vbo m_skyboxVBO;
        glrhi::vao m_skyboxVAO;
        glrhi::ebo m_skyboxEBO;
    };
}