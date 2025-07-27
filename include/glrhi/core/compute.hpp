/**
 * @file compute.hpp
 * @brief Handles the creation and usage of compute shaders.
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <unordered_map>
#include <string_view>

namespace glrhi{

    /**
     * @brief Handles the creation and usage of compute shaders.
     * 
     */
    class compute{
    public:

        /**
         * @brief Construct a new compute shader.
         * 
         * @param computePath Path to a text file containing the compute shader code in GLSL.
         */
        compute(const std::filesystem::path& computePath);
        ~compute();

        /**
         * @brief Dispatches the compute shader with the set number of invocations.
         * 
         * @param x Number of invocations in the x dimension.
         * @param y Number of invocations in the y dimension.
         * @param z Number of invocations in the z dimension.
         */
        void dispatch(GLuint x, GLuint y, GLuint z) const;

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

        /**
         * @brief Helper function to get uniform location while utilizing the cache.
         * 
         * @param name The name of the uniform.
         * @return Location of the uniform. 
         */
        GLint m_getLocation(const char* name);

        GLuint m_ID = 0;

        std::unordered_map<std::string_view, GLint> m_cache;
    };
}