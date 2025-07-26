/**
 * @file fbo.hpp
 * @brief Handles the creation and usage of Framebuffer objects.
 */

#pragma once

#include "glrhi/core/texture2D.hpp"

#include "glad/glad.h"

namespace glrhi {

    /**
     * @brief Handles the creation and usage of Framebuffer objects.
     * 
     */
    class fbo {
    public:

        /**
         * @brief Construct a new fbo object
         * 
         */
        fbo();
        ~fbo();

        /**
         * @brief Attaches a color texture to the FBO.
         *        The binding points start at 0 and incriment by 1 each function call.
         * @param texture The texture to attach.
         */
        void attachColorTexture(const glrhi::texture2D& texture);

        /**
         * @brief Attaches a depth texture to the FBO.
         *       
         * @param texture The texture to attach.
         *
         * @warning There can only be 1 depth texture attached,
         *           so calling this function multiple times will resoult in an error.
         */
        void attachDepthTexture(const glrhi::texture2D& texture);

        /**
         * @brief Inits the FBO with the textures added prior to the init.
         */
        void init() const;

        /**
         * @brief Binds the FBO for drawing.
         */
        void bind() const;

        /**
         * @brief Unbinds the FBO, I.e. binds the default framebuffer 0.
         * 
         */
        void unbind() const;
        
        /**
         * @brief Returns the FBO's OpenGL ID.
         * 
         * @return The FBO's OpenGL ID.
         */
        GLuint getID() const { return m_ID; }

    private:
        GLuint m_ID = 0;

        GLint m_colorTextures = 0;
        GLint m_maxColorTextures = 8;

        bool m_depthTexture = false;
    };
}