/**
 * @file shader.hpp
 * @brief Defines the Shader class for managing OpenGL shaders.
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

namespace glrhi{

    /**
     * @brief Manages OpenGL shaders, loading, compiling, and linking them.
     */
    class shader{
    public:

        /**
         * @brief Constructs a shader from vertex and fragment shader files.
         * @param vertexPath Path to the vertex shader file.
         * @param fragmentPath Path to the fragment shader file.
         */
        shader(const char* vertexPath, const char* fragmentPath);

        /**
         * @brief Destructor for the shader class.
         */
        ~shader();

        /**
         * @brief Activates the shader program for use.
         */
        void use() const;

        /**
         * @brief Sets a boolean uniform value.
         * @param name Name of the uniform variable.
         * @param value The boolean value to set.
         */
        void setBool(const char* name, bool value);

        /**
         * @brief Sets an integer uniform value.
         * @param name Name of the uniform variable.
         * @param value The integer value to set.
         */
        void setInt(const char* name, int value);

        /**
         * @brief Sets a float uniform value.
         * @param name Name of the uniform variable.
         * @param value The float value to set.
         */
        void setFloat(const char* name, float value);

        /**
         * @brief Sets a 2D vector uniform value.
         * @param name Name of the uniform variable.
         * @param value The 2D vector value to set.
         */
        void setVec2(const char* name, const glm::vec2& value);

        /**
         * @brief Sets a 3D vector uniform value.
         * @param name Name of the uniform variable.
         * @param value The 3D vector value to set.
         */
        void setVec3(const char* name, const glm::vec3& value);

        /**
         * @brief Sets a 4D vector uniform value.
         * @param name Name of the uniform variable.
         * @param value The 4D vector value to set.
         */
        void setVec4(const char* name, const glm::vec4& value);

        /**
         * @brief Sets a 2x2 matrix uniform value.
         * @param name Name of the uniform variable.
         * @param mat The 2x2 matrix value to set.
         */
        void setMat2(const char* name, const glm::mat2& mat);

        /**
         * @brief Sets a 3x3 matrix uniform value.
         * @param name Name of the uniform variable.
         * @param mat The 3x3 matrix value to set.
         */
        void setMat3(const char* name, const glm::mat3& mat);
        
        /**
         * @brief Sets a 4x4 matrix uniform value.
         * @param name Name of the uniform variable.
         * @param mat The 4x4 matrix value to set.
         */
        void setMat4(const char* name, const glm::mat4& mat);

    private:
        GLint m_getLocation(const char* name);

        GLuint m_ID;

        std::unordered_map<std::string_view, GLint> m_cache;
    };
}