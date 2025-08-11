#pragma once

#include "glrhi/core/shader.hpp"
#include "glrhi/core/texture2D.hpp"
#include <glrhi/glrhi.hpp>

namespace glrhi {
    class gbuffer {
    public:

        gbuffer();
        ~gbuffer() = default;

        void bind();
        void unbind();

        void bindTextures(glrhi::shader& shader);

        void renderResoult();

    private:

        glrhi::vao m_quadVAO;
        glrhi::vbo m_quadVBO;
        glrhi::ebo m_quadEBO;

        glrhi::fbo m_gbuffer;

        glrhi::texture2D m_albedo;
        glrhi::texture2D m_normalRoughness;
        glrhi::texture2D m_emissionMetallic;
        glrhi::texture2D m_position;
        glrhi::texture2D m_resoult;

        glrhi::texture2D m_depth;

        glrhi::shader m_resoultShader;

    };
}