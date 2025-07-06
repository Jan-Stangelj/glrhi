/**
 * @file fbo.hpp
 * @brief Defines the Framebuffer Object (FBO) class.
 */

#pragma once

#include "glrhi/core/texture2D.hpp"

#include "glad/glad.h"

#include <array>
#include <memory>

namespace glrhi {

    /**
     * @brief Represents a Framebuffer Object (FBO).
     */
    class fbo {
    public:

        /**
         * @brief Constructs a new fbo object.
         */
        fbo();

        /**
         * @brief Destroys the fbo object.
         */
        ~fbo();

        /**
         * @brief Attaches a color texture to the FBO.
         *
         * @param texture A pointer to the texture2D object to attach.
         * @warning The lifetime of the provided texture must be managed carefully.  The texture must remain valid as long as the FBO is in use. Failing to do so will result in undefined behavior.
         */
        void attachColorTexture(glrhi::texture2D* texture);

        /**
         * @brief Attaches a depth texture to the FBO.
         *
         * @param texture A pointer to the texture2D object to attach.
         * @warning The lifetime of the provided texture must be managed carefully.  The texture must remain valid as long as the FBO is in use. Failing to do so will result in undefined behavior.
         */
        void attachDepthTexture(glrhi::texture2D* texture);

        /**
         * @brief Initializes the FBO.
         */
        void init();

        /**
         * @brief Binds the FBO for drawing.
         */
        void bind();

        /**
         * @brief Unbinds the FBO.
         */
        void unbind();

        /**
         * @brief Returns the framebuffer object's ID.
         *
         * @return The framebuffer object's ID.
         */
        GLuint getID() { return m_ID; }

    private:
        GLuint m_ID = 0;

        std::array<glrhi::texture2D*, 8> m_colorTextures;
        unsigned int m_numColorTextures = 0;

        glrhi::texture2D* m_depthTexture = nullptr;
    };
}