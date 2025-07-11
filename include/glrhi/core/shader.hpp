#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

namespace glrhi{
    class shader{
    public:

        /**
         * @brief Constructs a shader from vertex and fragment shader files.
         * @param vertexPath Path to the vertex shader file.
         * @param fragmentPath Path to the fragment shader file.
         */
        shader(const char* vertexPath, const char* fragmentPath);
        ~shader();

        void use() const;

        void setBool(const char* name, bool value);
        void setInt(const char* name, int value);
        void setFloat(const char* name, float value);
        void setVec2(const char* name, const glm::vec2& value);
        void setVec3(const char* name, const glm::vec3& value);
        void setVec4(const char* name, const glm::vec4& value);
        void setMat2(const char* name, const glm::mat2& mat);
        void setMat3(const char* name, const glm::mat3& mat);
        void setMat4(const char* name, const glm::mat4& mat);

    private:
        GLint m_getLocation(const char* name);

        GLuint m_ID;

        std::unordered_map<std::string_view, GLint> m_cache;
    };
}