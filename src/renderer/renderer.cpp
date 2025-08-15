#include "glrhi/core/ebo.hpp"
#include <glrhi/renderer.hpp>

#include <glrhi/renderer/shaders.hpp>


#include <iostream>
namespace glrhi {
    renderer::renderer(unsigned int width, unsigned int height, const char* title)  : m_window(width, height, title), m_gBuffer(width, height) { 
        m_gBufferShader.createFromCode(glrhi::vertexGbufferCode.data(), glrhi::fragmentGbufferCode.data()); 
        m_lightingShader.createFromCode(glrhi::lightingShaderCode.data());
        m_skyboxShader.createFromCode(skyboxVert.data(), skyboxFrag.data());
        m_width = width; 
        m_height = height; 
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_CULL_FACE);

        m_skyboxVBO.create(skyboxVertices, sizeof(skyboxVertices));
        m_skyboxEBO.create(skyboxIndices, sizeof(skyboxIndices));

        m_skyboxVAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
        m_skyboxVAO.init(m_skyboxVBO, m_skyboxEBO, sizeof(float) * 3);
    }

    void renderer::gBufferPass(const glrhi::scene& scene) {
        m_camera.uploadData();
        m_camera.bind();

        m_gBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_gBufferShader.use();

        scene.drawModels(m_gBufferShader);

        if (scene.hasSkybox()) {
            glDepthFunc(GL_EQUAL);

            m_skyboxShader.use();
            scene.bindSkybox(m_skyboxShader);

            m_skyboxVAO.bind();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

            glDepthFunc(GL_LESS);
        }
    }

    void renderer::lightingPass(const glrhi::scene& scene) {
        scene.updateLightBuffer(m_lightingShader);

        m_gBuffer.bindTexturesLightingPass(m_lightingShader);
        scene.bindSkybox(m_lightingShader);
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