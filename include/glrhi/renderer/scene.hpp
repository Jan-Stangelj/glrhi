#pragma once

#include "glrhi/core/shader.hpp"
#include "glrhi/core/ssbo.hpp"
#include "glrhi/renderer/lighting.hpp"
#include <glrhi/glrhi.hpp>

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

        void drawModels(glrhi::shader& shader) const;
        void updateLightBuffer(glrhi::compute& shader) const;

    private:

        std::vector<glrhi::model> m_models;
        std::vector<glrhi::light> m_lights;

        glrhi::ssbo m_lightBuffer;

    };
}