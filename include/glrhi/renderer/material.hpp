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

        /**
         * @brief Binds the material in the specified shader.
         * 
         * @param shader The shader to bind the material in.
         */
        void bind(glrhi::shader& shader) const;


        /**
         * @brief Loads the albedo texture.
         * 
         * @param path Path to the texture.
         */
        void setAlbedoTexture(const std::filesystem::path& path);

        /**
         * @brief Loads the ARM texture.
         * 
         * @param path Path to the texture.
         */
        void setARMtexture(const std::filesystem::path& path);

        /**
         * @brief Loads the normal texture.
         * 
         * @param path Path to the texture.
         */
        void setNormalTexture(const std::filesystem::path& path);

        /**
         * @brief Loads the emission texture.
         * 
         * @param path Path to the texture.
         */
        void setEmissionTexture(const std::filesystem::path& path);

        /**
         * @brief Fallback albedo, used if no texture is provided.
         */
        glm::vec4 albedo = glm::vec4(1.0f);

        /**
         * @brief Fallback ARM, used if no texture is provided.
         */
        glm::vec4 arm = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);

        /**
         * @brief Fallback emission, used if no texture is provided.
         */
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