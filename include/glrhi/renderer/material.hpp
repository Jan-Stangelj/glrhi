#pragma once

#include "glrhi/core/shader.hpp"
#include <glrhi/glrhi.hpp>

#include <glm/glm.hpp>

#include <filesystem>

namespace glrhi {
    class material {
    public:

        material();
        ~material() = default;

        void bind(glrhi::shader& shader) const;

        void setAlbedoTexture(const std::filesystem::path& path);
        void setARMtexture(const std::filesystem::path& path);
        void setNormalTexture(const std::filesystem::path& path);
        void setEmissionTexture(const std::filesystem::path& path);

        glm::vec4 albedo = glm::vec4(1.0f);
        glm::vec4 arm = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        glm::vec4 emission = glm::vec4(0.0f);

    private:

        glrhi::texture2D m_albedoTex;
        glrhi::texture2D m_armTex;
        glrhi::texture2D m_normalTex;
        glrhi::texture2D m_emissionTex;

        float m_hasAlbedoTex = false;
        float m_hasARMtex = false;
        float m_hasNormalTex = false;
        float m_hasEmissionTex = false;

        glrhi::ubo m_matBuffer;
    };
}