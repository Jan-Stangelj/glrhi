#include "glm/fwd.hpp"
#include "glrhi/core/ebo.hpp"
#include <filesystem>
#include <glrhi/renderer/scene.hpp>

namespace glrhi {
    scene::scene() : m_lightBuffer((sizeof(glrhi::light) + 12) * 65) {}

    unsigned int scene::addModel(const std::filesystem::path& path) {
        m_models.emplace_back(path);
        return m_models.size() - 1;
    }
    glrhi::model& scene::getModel(unsigned int id) {
        return m_models[id];
    }

    unsigned int scene::addLight() {
        m_lights.emplace_back();
        return m_lights.size() - 1;
    }
    glrhi::light& scene::getLight(unsigned int id) {
        return m_lights[id];
    }

    void scene::setSkybox(const std::filesystem::path& path) {
        m_skybox.create(path, GL_RGB16F);
        m_hasSkybox = true;
    }
    void scene::bindSkybox(unsigned int unit) const {
        m_skybox.bind(unit);
    }

    void scene::drawModels(glrhi::shader& shader) const {
        for (auto const& model : m_models) {
            model.draw(shader);
        }
    }

    void scene::updateLightBuffer(glrhi::compute& shader) const {
        int numLights = m_lights.size();
        shader.setInt("u_numLights", numLights);

        m_lightBuffer.sendData(0, sizeof(glm::vec4), &sunDir);
        m_lightBuffer.sendData(sizeof(glm::vec4), sizeof(glm::vec4), &sunColor);
        m_lightBuffer.sendData(2*sizeof(glm::vec4), sizeof(float), &sunStrenght);

        unsigned int i = 1;
        for (auto const& light : m_lights) {
            size_t lightSize = sizeof(glrhi::light) + 12;
            m_lightBuffer.sendData(lightSize * i, lightSize, &light);
            i++;
        }

        m_lightBuffer.addBindingPoint(0);
    }
}