#include "glrhi/core/shader.hpp"
#include "glrhi/renderer/lighting.hpp"
#include <glrhi/renderer.hpp>

#include <iostream>

namespace glrhi {
    void renderer::gBufferPass() {
        m_camera.uploadData();
        m_camera.bind();

        m_gBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_gBufferShader.use();

        for (auto const& model : m_models) {
            model.second.draw(m_gBufferShader);
        }
    }

    void renderer::lightingPass() {
        int numLights = m_lights.size();
        

        unsigned int i = 0;
        for (auto const& light : m_lights) {
            size_t lightSize = sizeof(glrhi::light);
            m_lightBuffer.sendData(lightSize * i, lightSize, &light.second);
            i++;
        }

        m_lightBuffer.addBindingPoint(0);

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