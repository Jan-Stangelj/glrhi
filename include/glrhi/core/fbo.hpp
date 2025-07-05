#pragma once

#include "glrhi/core/texture2D.hpp"

#include "glad/glad.h"

#include <array>
#include <memory>

namespace glrhi {
    class fbo {
    public:

        fbo();
        ~fbo();

        void attachColorTexture(glrhi::texture2D* texture);
        void attachDepthTexture(glrhi::texture2D* texture);

        void init();

        void bind();
        void unbind();

        GLuint getID() { return m_ID; }

    private:

        GLuint m_ID = 0;

        std::array<glrhi::texture2D*, 8> m_colorTextures;
        unsigned int m_numColorTextures = 0;

        glrhi::texture2D* m_depthTexture;

    };
}