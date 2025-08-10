#include "glrhi/core/ebo.hpp"
#include <filesystem>
#include <glrhi/renderer/material.hpp>

namespace glrhi {
    material::material() {
        //                           < scalar values       >    < bool "has" values (float for alignment)
        constexpr GLsizeiptr size = (sizeof(GLfloat) * 4 * 3) + sizeof(GLfloat) * 4;
        m_matBuffer.create(size);
    }

    void material::bind(glrhi::shader& shader) const {
        m_matBuffer.addBindingPoint(1);

        m_matBuffer.sendData(0, sizeof(GLfloat) * 4, &albedo);
        m_matBuffer.sendData(sizeof(GLfloat) * 4, sizeof(GLfloat) * 4, &arm);
        m_matBuffer.sendData(sizeof(GLfloat) * 8, sizeof(GLfloat) * 4, &emission);

        m_matBuffer.sendData(sizeof(GLfloat) * 12, sizeof(GLfloat), &m_hasAlbedoTex);
        m_matBuffer.sendData(sizeof(GLfloat) * 13, sizeof(GLfloat), &m_hasARMtex);
        m_matBuffer.sendData(sizeof(GLfloat) * 14, sizeof(GLfloat), &m_hasNormalTex);
        m_matBuffer.sendData(sizeof(GLfloat) * 15, sizeof(GLfloat), &m_hasEmissionTex);

        m_albedoTex.bind(0);
        m_armTex.bind(1);
        m_normalTex.bind(2);
        m_emissionTex.bind(3);

        shader.setInt("u_albedo", 0);
        shader.setInt("u_arm", 1);
        shader.setInt("u_normal", 2);
        shader.setInt("u_emission", 3);
    }

    void material::setAlbedoTexture(const std::filesystem::path& path) {
        m_albedoTex.create(path, GL_RGBA8);
    }

    void material::setARMtexture(const std::filesystem::path& path) {
        m_armTex.create(path, GL_RGB8);
    }

    void material::setNormalTexture(const std::filesystem::path& path) {
        m_normalTex.create(path, GL_RGB8);
    }

    void material::setEmissionTexture(const std::filesystem::path& path) {
        m_emissionTex.create(path, GL_RGB8);
    }
}