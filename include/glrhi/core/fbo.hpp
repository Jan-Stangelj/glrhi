#pragma once

#include "glrhi/glrhi.hpp"

#include "glad/glad.h"

#include <array>
#include <memory>

namespace glrhi {
    class texture2D;

    class fbo {
    public:

        fbo();
        ~fbo();

        void attachColorTexture(const std::shared_ptr<glrhi::texture2D>& texture);
        void attachDepthTexture(const std::shared_ptr<glrhi::texture2D>& texture);

        void init();

        void bind();
        void unbind();

        GLuint getID() { return m_ID; }

    private:

        GLuint m_ID = 0;

        // Yes, I know, this is dogshit
        std::array<std::shared_ptr<glrhi::texture2D>, 8> m_colorTextures;
        unsigned int m_numColorTextures = 0;

        std::shared_ptr<glrhi::texture2D> m_depthTexture = nullptr;

    };
}