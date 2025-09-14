#include "glrhi/core/shader.hpp"
#include "glrhi/renderer/constants.hpp"
#include <glrhi/renderer/gbuffer.hpp>

namespace glrhi {
    gbuffer::gbuffer(unsigned int width, unsigned int height) {
        m_loadQuad();
        m_initFramebuffer(width, height);
        m_resoultShader.create("../shaders/resoult.vert", "../shaders/resoult.frag");
    }

    void gbuffer::bind() {
        m_gBuffer.bind();
    }

    void gbuffer::unbind() {
        m_gBuffer.unbind();
    }

    void gbuffer::renderResoult() {
        unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_resoultShader.use();
        m_quadVAO.bind();
        resoult.bind(0);
        m_resoultShader.setInt("u_resoult", 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void gbuffer::m_loadQuad() {
        m_quadVBO.create(quadVertices, sizeof(quadVertices));
        m_quadEBO.create(quadIndices, sizeof(quadIndices));

        m_quadVAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
        m_quadVAO.addAttribute(2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
        m_quadVAO.init(m_quadVBO, m_quadEBO, 5*sizeof(float));
    }

    void gbuffer::m_initFramebuffer(unsigned int width, unsigned int height) {
        albedo.create(width, height, GL_RGBA16F);
        normalRoughness.create(width, height, GL_RGBA16F);
        emissionMetallic.create(width, height, GL_RGBA8);
        position.create(width, height, GL_RGB16F);
        resoult.create(width, height, GL_RGBA16F);

        depth.create(width, height, GL_DEPTH_COMPONENT24);

        m_gBuffer.attachColorTexture(albedo);
        m_gBuffer.attachColorTexture(normalRoughness);
        m_gBuffer.attachColorTexture(emissionMetallic);
        m_gBuffer.attachColorTexture(position);
        m_gBuffer.attachColorTexture(resoult);
        m_gBuffer.attachDepthTexture(depth);

        m_gBuffer.init();
    }
}