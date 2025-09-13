/**
 * @file shader.hpp
 * @brief Handles the creation and usage of shaders.
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <utility>

namespace glrhi {

    /**
     * @brief Handles the creation and usage of shaders.
     */
    class shader {
    public:
        shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geomPath = "");
        shader() = default;
        ~shader();

        shader(const glrhi::shader&) = delete;
        glrhi::shader& operator=(const glrhi::shader&) = delete;

        shader(glrhi::shader&& other) noexcept {
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        glrhi::shader& operator=(glrhi::shader&& other) {
            if (this != &other) {
                if (m_ID)
                    glDeleteProgram(m_ID);
                m_ID = other.m_ID;
                m_cache = std::move(other.m_cache);
                other.m_ID = 0;
            }
            return *this;
        }

        void create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geomPath = "");
        void createFromCode(const char* vertexCode, const char* fragmentCode, const char* geomCode = nullptr);

        void use() const;

        void setBool(const char* name, bool value);
        void setInt(const char* name, int value);
        void setFloat(const char* name, float value);

        void setVec2(const char* name, const glm::vec2& value);
        void setVec3(const char* name, const glm::vec3& value);
        void setVec4(const char* name, const glm::vec4& value);

        void setUVec2(const char* name, const glm::uvec2& value);
        void setUVec3(const char* name, const glm::uvec3& value);
        void setUVec4(const char* name, const glm::uvec4& value);

        void setMat2(const char* name, const glm::mat2& mat);
        void setMat3(const char* name, const glm::mat3& mat);
        void setMat4(const char* name, const glm::mat4& mat);

        void setHandle64(const char* name, const GLuint64& handle);

    private:
        GLint m_getLocation(const char* name);

        GLuint m_ID = 0;
        std::unordered_map<std::string_view, GLint> m_cache;
    };
}
