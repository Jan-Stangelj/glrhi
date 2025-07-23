#pragma once

#include "glrhi/core/texture2D.hpp"

#include "glad/glad.h"

namespace glrhi {
    class fbo {
    public:

        fbo();
        ~fbo();

        void attachColorTexture(const glrhi::texture2D& texture);
        void attachDepthTexture(const glrhi::texture2D& texture);

        /// @brief Inits the fbo. Textures must be added before init.
        void init() const;

        void bind() const;
        void unbind() const;
        
        GLuint getID() const { return m_ID; }

    private:
        GLuint m_ID = 0;

        unsigned short m_colorTextures = 0;
        unsigned short m_maxColorTextures = 8;

        bool m_depthTexture = false;
    };
}