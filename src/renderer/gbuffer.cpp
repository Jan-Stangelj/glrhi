#include "glrhi/core/shader.hpp"
#include <glrhi/renderer/gbuffer.hpp>

namespace glrhi {
    gbuffer::gbuffer(unsigned int width, unsigned int height) {
        m_loadQuad();
        m_initFramebuffer(width, height);
        m_loadShader();
    }

    void gbuffer::bind() {
        m_gBuffer.bind();
    }

    void gbuffer::unbind() {
        m_gBuffer.unbind();
    }

    void gbuffer::bindTextures(glrhi::shader& shader) {
        m_albedo.bind(0);
        shader.setInt("u_albedo", 0);

        m_normalRoughness.bind(1);
        shader.setInt("u_normalRoughness", 1);

        m_emissionMetallic.bind(2);
        shader.setInt("u_emissionMetallic", 2);

        m_position.bind(3);
        shader.setInt("u_position", 3);

        m_resoult.bindImage(0, 0);
    }

    void gbuffer::bindTextures(glrhi::compute& shader) {
        m_albedo.bind(0);
        shader.setInt("u_albedo", 0);

        m_normalRoughness.bind(1);
        shader.setInt("u_normalRoughness", 1);

        m_emissionMetallic.bind(2);
        shader.setInt("u_emissionMetallic", 2);

        m_position.bind(3);
        shader.setInt("u_position", 3);

        m_resoult.bindImage(0, 0);
    }

    void gbuffer::bindTexturesLightingPass(glrhi::compute& shader) {
        m_albedo.bindImage(1, 0);

        m_normalRoughness.bind(1);
        shader.setInt("u_normalRoughness", 1);

        m_emissionMetallic.bind(2);
        shader.setInt("u_emissionMetallic", 2);

        m_position.bind(3);
        shader.setInt("u_position", 3);

        m_resoult.bindImage(0, 0);
    }

    void gbuffer::renderResoult() {
        unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_resoultShader.use();
        m_quadVAO.bind();
        m_resoult.bind(0);
        m_resoultShader.setInt("u_resoult", 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void gbuffer::m_loadQuad() {
        float quad[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // zgoraj levo
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // zgoraj desno
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // spodaj levo
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f // spodaj desno
        };

        unsigned int quadIndices[] = {
            0, 2, 1,
            1, 2, 3
        };

        m_quadVBO.create(quad, sizeof(quad));
        m_quadEBO.create(quadIndices, sizeof(quadIndices));

        m_quadVAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
        m_quadVAO.addAttribute(2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
        m_quadVAO.init(m_quadVBO, m_quadEBO, 5*sizeof(float));
    }

    void gbuffer::m_initFramebuffer(unsigned int width, unsigned int height) {
        m_albedo.create(width, height, GL_RGB8);
        m_normalRoughness.create(width, height, GL_RGBA16F);
        m_emissionMetallic.create(width, height, GL_RGBA8);
        m_position.create(width, height, GL_RGB16F);
        m_resoult.create(width, height, GL_RGBA16F);

        m_depth.create(width, height, GL_DEPTH_COMPONENT24);

        m_gBuffer.attachColorTexture(m_albedo);
        m_gBuffer.attachColorTexture(m_normalRoughness);
        m_gBuffer.attachColorTexture(m_emissionMetallic);
        m_gBuffer.attachColorTexture(m_position);
        m_gBuffer.attachColorTexture(m_resoult);
        m_gBuffer.attachDepthTexture(m_depth);

        m_gBuffer.init();
    }

    void gbuffer::m_loadShader() {
        const char* vertCode = "#version 460 core\n"
                                "\n"
                                "layout(location = 0) in vec3 a_pos;\n"
                                "layout(location = 1) in vec2 a_texUV;\n"
                                "\n"
                                "out vec2 texUV;\n"
                                "\n"
                                "void main() {\n"
                                "\tgl_Position = vec4(a_pos, 1.0);\n"
                                "\ttexUV = a_texUV;\n"
                                "}";

        const char* fragCode = "#version 460 core\n"
                                "\n"
                                "out vec4 fragColor;\n"
                                "\n"
                                "in vec2 texUV;\n"
                                "\n"
                                "uniform sampler2D u_resoult;\n"
                                "\n"
                                "void main() {\n"
                                "\tfragColor = vec4(texture(u_resoult, texUV).xyz, 1.0);\n"
                                "}";

        m_resoultShader.createFromCode(vertCode, fragCode);
    }
}