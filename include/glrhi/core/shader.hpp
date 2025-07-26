/**
 * @file shader.hpp
 * @brief Handles the creation and usage of shaders.
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string_view>
#include <filesystem>

namespace glrhi{

    /**
     * @brief Handles the creation and usage of shaders.
     */
    class shader{
    public:

        /**
         * @brief Construct a new shader.
         * 
         * @param vertexPath Path to a text file containing the vertex shader code in GLSL.
         * @param fragmentPath Path to a text file containing the vertex shader code in GLSL.
         */
        shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        ~shader();

        /**
         * @brief Selects the shader to be used in the subsequent draw calls.
         */
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

        /**
         * @brief Sets a texture handle for use with bindless textures.
         * 
         * @param name Name of the uniform to set to.
         * @param handle The texture handle.
         */
        void setHandle64(const char* name, const GLuint64& handle);

    private:
        GLint m_getLocation(const char* name);

        GLuint m_ID;

        std::unordered_map<std::string_view, GLint> m_cache;
    };
}