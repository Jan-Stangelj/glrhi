#pragma once

#include <glrhi/glrhi.hpp>

#include "glm/fwd.hpp"
#include "glrhi/core/shader.hpp"
#include "glrhi/core/ssbo.hpp"
#include "glrhi/core/texture2D.hpp"
#include "glrhi/renderer/lighting.hpp"
#include <glrhi/renderer/model.hpp>

#include <filesystem>
#include <vector>

namespace glrhi {
    class scene {
    public:

        scene();
        ~scene() = default;

        unsigned int addModel(const std::filesystem::path& path);
        glrhi::model& getModel(unsigned int id);

        unsigned int addLight();
        glrhi::light& getLight(unsigned int id);

        void setSkybox(const std::filesystem::path& path);
        void bindSkybox(glrhi::shader& shader) const;
        void bindSkybox(glrhi::compute& shader) const;
        bool hasSkybox() const { return m_hasSkybox; };

        void drawModels(glrhi::shader& shader) const;
        void updateLightBuffer(glrhi::compute& shader) const;

        glm::vec4 sunDir = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
        glm::vec4 sunColor = glm::vec4(1.0f);
        float sunStrenght = 5.0f;

    private:

        std::vector<glrhi::model> m_models;
        std::vector<glrhi::light> m_lights;

        glrhi::texture2D m_skybox;
        bool m_hasSkybox = false;

        glrhi::ssbo m_lightBuffer;

    };
}