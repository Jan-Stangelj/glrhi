#include "glrhi/core/fbo.hpp"

#include <iostream>
#include <vector>

namespace glrhi {

    fbo::fbo() {
        glCreateFramebuffers(1, &m_ID);
    }

    fbo::~fbo() {
        glDeleteFramebuffers(1, &m_ID);
    }

    void fbo::attachColorTexture(glrhi::texture2D* texture) {
        if (!(m_numColorTextures < m_colorTextures.size())) {
            std::cerr << "ERROR::FRAMEBUFFER::COLOR_TEXTURE_ATTACHMENT_FAILED:\n"
                        "Number of texture attachments exceeded maximum (" << m_colorTextures.size() << ")\n";
            return;
        }
        m_colorTextures[m_numColorTextures] = texture;

        glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + m_numColorTextures, texture->getID(), 0);

        m_numColorTextures++;
    }

    void fbo::attachDepthTexture(glrhi::texture2D* texture) {
        if (m_depthTexture != nullptr) {
            std::cerr << "ERROR::FRAMEBUFFER::DEPTH_TEXTURE_ATTACHMENT_FAILED:\n"
                        "Depth texture already attached\n";
            return;
        }
        m_depthTexture = texture;

        glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, texture->getID(), 0);
    }

    void fbo::init() const {

        // Adds the color buffers to a list, to tell OpenGL how many there are
        // (this is a weird system by used by OGL imho)
        std::vector<GLenum> drawBuffers;
        for (unsigned int i = 0; i < m_numColorTextures; ++i) {
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
        }

        glNamedFramebufferDrawBuffers(m_ID, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());

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
