#include <glrhi/renderer.hpp>

#include <glrhi/renderer/shaders.hpp>

namespace glrhi {
    renderer::renderer(unsigned int width, unsigned int height, const char* title)  : m_window(width, height, title), m_gBuffer(width, height) { 
        m_gBufferShader.createFromCode(m_vertexGbufferCode.data(), m_fragmentGbufferCode.data()); 
        m_lightingShader.create("../examples/renderer/shaders/lighting.comp"); 
        m_width = width; 
        m_height = height; 
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_CULL_FACE); 
    }

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