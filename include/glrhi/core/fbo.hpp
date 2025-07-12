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

        /**
         * @brief Attaches a color texture to the FBO.
         *
         * @param texture A pointer to the texture2D object to attach.
         * @warning The lifetime of the provided texture must be managed carefully. The texture must remain valid as long as the FBO is in use. Failing to do so will result in undefined behavior.
         */
        void attachColorTexture(glrhi::texture2D* texture);

        /**
         * @brief Attaches a depth texture to the FBO.
         *
         * @param texture A pointer to the texture2D object to attach.
         * @warning The lifetime of the provided texture must be managed carefully. The texture must remain valid as long as the FBO is in use. Failing to do so will result in undefined behavior.
         */
        void attachDepthTexture(glrhi::texture2D* texture);

        void init() const;

        void bind() const;
        void unbind() const;
        
        GLuint getID() const { return m_ID; }

    private:
        GLuint m_ID = 0;

        std::array<glrhi::texture2D*, 8> m_colorTextures;
        unsigned int m_numColorTextures = 0;

        glrhi::texture2D* m_depthTexture = nullptr;
    };
}