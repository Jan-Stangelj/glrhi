#include "glrhi/core/ebo.hpp"
#include <glrhi/renderer.hpp>

#include <glrhi/renderer/constants.hpp>

namespace glrhi {
    renderer::renderer(unsigned int width, unsigned int height, const char* title)  : m_window(width, height, title), m_gBuffer(width, height) { 
        m_gBufferShader.create("../shaders/gbuffer.vert", "../shaders/gbuffer.frag"); 
        m_lightingShader.create("../shaders/lighting.comp");
        m_skyboxShader.create("../shaders/skybox.vert", "../shaders/skybox.frag");
        m_width = width; 
        m_height = height; 

        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_CULL_FACE);

        m_skyboxVBO.create(cubeVertices, sizeof(cubeVertices));
        m_skyboxEBO.create(cubeIndices, sizeof(cubeIndices));

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
            glDisable(GL_DEPTH_TEST);

            m_skyboxShader.use();
            scene.bindSkybox(0);

            m_skyboxVAO.bind();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

            glDepthFunc(GL_LESS);
            glEnable(GL_DEPTH_TEST);
        }
    }

    void renderer::lightingPass(const glrhi::scene& scene) {
        scene.updateLightBuffer(m_lightingShader);

        m_gBuffer.albedo.bind(0);
        m_gBuffer.normalRoughness.bind(1);
        m_gBuffer.emissionMetallic.bind(2);
        m_gBuffer.position.bind(3);
        scene.bindSkybox(4);

        m_gBuffer.resoult.bindImage(0, 0);

        m_lightingShader.dispatch(m_width, m_height, 1);
    }

    void renderer::renderResoult() {
        m_gBuffer.renderResoult();
        m_window.swapBuffers();
    }
}