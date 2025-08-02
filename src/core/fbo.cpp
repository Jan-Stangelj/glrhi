#include "glrhi/core/fbo.hpp"
#include "glrhi/core/texture2D.hpp"

#include <iostream>
#include <vector>

namespace glrhi {

    fbo::fbo() {
        glCreateFramebuffers(1, &m_ID);

        // Get max number of attachments that can be drawn to
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_maxColorTextures);
    }

    fbo::~fbo() {
        glDeleteFramebuffers(1, &m_ID);
    }

    void fbo::attachColorTexture(const glrhi::texture2D& texture) {
        // Logs an error if the maximum number of color textures are already attached.
        if (!(m_colorTextures < m_maxColorTextures)) {
            std::cerr << "ERROR::FRAMEBUFFER::COLOR_TEXTURE_ATTACHMENT_FAILED:\n"
                        "Number of texture attachments exceeded maximum (" << m_colorTextures << ")\n";
            return;
        }

        glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + m_colorTextures, texture.getID(), 0);

        m_colorTextures++;
    }

    void fbo::attachDepthTexture(const glrhi::texture2D& texture) {
        // Logs an error if a depth texture is already attached.
        if (m_depthTexture == true) {
            std::cerr << "ERROR::FRAMEBUFFER::DEPTH_TEXTURE_ATTACHMENT_FAILED:\n"
                        "Depth texture already attached\n";
            return;
        }

        glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, texture.getID(), 0);

        m_depthTexture = true;
    }

    void fbo::init() const {

        // Adds the color buffers to a list, to tell OpenGL how many there are
        // (this is a weird system by used by OGL imho)
        std::vector<GLenum> drawBuffers;
        for (GLint i = 0; i < m_colorTextures; i++) {
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
        }

        glNamedFramebufferDrawBuffers(m_ID, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());

        // Checks if the framebuffer was initialized correctly
        GLenum status = glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR::FRAMEBUFFER::INIT_FAILED\n"
                        "Framebuffer is not complete\n";
        }
    }

    void fbo::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    }

    void fbo::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
