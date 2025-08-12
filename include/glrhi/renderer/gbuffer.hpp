/**
 * @file gbuffer.hpp
 * @brief Contains the abstraction class for the gBuffer.
 */

#pragma once

#include "glrhi/core/shader.hpp"
#include "glrhi/core/texture2D.hpp"
#include <glrhi/glrhi.hpp>

namespace glrhi {

    /**
     * @brief Abstraction for the gbuffer.
     *
     * @details Contains the following buffers:
     *          Albedo, NormalRoughness, EmissionMetallic, Position, Resoult.
     */
    class gbuffer {
    public:

        /**
         * @brief Construct a new gbuffer with the specified width and height.
         * 
         * @param width Width of the gbuffer.
         * @param height Height of the gbuffer.
         */
        gbuffer(unsigned int width, unsigned int height);
        ~gbuffer() = default;

        /**
         * @brief Binds the gbuffer for rendering.
         */
        void bind();

        /**
         * @brief Unbinds the gbuffer, ie. binds the default framebuffer.
         */
        void unbind();

        /**
         * @brief Binds all the textures in the gbuffer.
         * 
         * @param shader the shader to bind them in.
         *
         * @details It binds the following sampler2D uniforms:
         *          u_albedo, u_normalRoughness, u_emissionMetallic, u_position, u_resoult.
         */
        void bindTextures(glrhi::shader& shader);

        /**
         * @brief Binds all the textures in the gbuffer.
         * 
         * @param shader the shader to bind them in.
         *
         * @details It binds the following sampler2D uniforms:
         *          u_albedo, u_normalRoughness, u_emissionMetallic, u_position.
         *          And the following image2D uniforms:
         *          u_resoult
         */
        void bindTextures(glrhi::compute& shader);

        /**
         * @brief Not meant for end user
         */
        void bindTexturesLightingPass(glrhi::compute& shader);

        /**
         * @brief Binds the default framebuffer and renders the contents of the resoult texture to the screen.
         */
        void renderResoult();

    private:

        glrhi::vao m_quadVAO;
        glrhi::vbo m_quadVBO;
        glrhi::ebo m_quadEBO;

        glrhi::fbo m_gBuffer;

        glrhi::texture2D m_albedo;
        glrhi::texture2D m_normalRoughness;
        glrhi::texture2D m_emissionMetallic;
        glrhi::texture2D m_position;
        glrhi::texture2D m_resoult;

        glrhi::texture2D m_depth;

        glrhi::shader m_resoultShader;

        void m_loadQuad();
        void m_initFramebuffer(unsigned int width, unsigned int height);
        void m_loadShader();

    };
}