#pragma once

#include <glrhi/glrhi.hpp>

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
        bool hasSkybox() const { return m_hasSkybox; };

        void drawModels(glrhi::shader& shader) const;
        void updateLightBuffer(glrhi::compute& shader) const;

    private:

        std::vector<glrhi::model> m_models;
        std::vector<glrhi::light> m_lights;

        glrhi::texture2D m_skybox;
        bool m_hasSkybox = false;

        glrhi::ssbo m_lightBuffer;

    };
}