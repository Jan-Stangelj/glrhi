#pragma once

#include <glrhi/glrhi.hpp>

#include <glm/glm.hpp>

#include <filesystem>

namespace glrhi {
    class material {
    public:

        material();
        ~material();

        void bind();

        void setAlbedoTexture(const std::filesystem::path& path);
        void setARMtexture(const std::filesystem::path& path);
        void setNormalTexture(const std::filesystem::path& path);
        void setEmissionTexture(const std::filesystem::path& path);

        glm::vec4 albedo = glm::vec4(1.0f);
        glm::vec3 arm = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 emission = glm::vec3(0.0f);

    private:

        glrhi::texture2D m_albedoTex;
        glrhi::texture2D m_armTex;
        glrhi::texture2D m_normalTex;
        glrhi::texture2D m_emissionTex;

        bool m_hasAlbedoTex = false;
        bool m_hasARMtex = false;
        bool m_hasNormalTex = false;
        bool m_hasEmissionTex = false;

        glrhi::ubo m_materialBuffer;

    };
}