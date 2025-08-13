#include "glrhi/core/shader.hpp"
#include "glrhi/renderer/lighting.hpp"
#include <glrhi/renderer.hpp>

#include <iostream>

namespace glrhi {
    void renderer::gBufferPass(const glrhi::scene& scene) {
        m_camera.uploadData();
        m_camera.bind();

        m_gBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_gBufferShader.use();

        scene.drawModels(m_gBufferShader);
    }

    void renderer::lightingPass(const glrhi::scene& scene) {
        scene.updateLightBuffer(m_lightingShader);

        m_gBuffer.bindTexturesLightingPass(m_lightingShader);
        m_lightingShader.dispatch(m_width, m_height, 1);
    }

    void renderer::postProcessPass() {
        for (auto& shader : m_postProcessShaders) {
            m_gBuffer.bindTextures(shader);
            shader.dispatch(m_width, m_height, 1);
        }
    }

    void renderer::renderResoult() {
        m_gBuffer.renderResoult();
        m_window.swapBuffers();
    }
}