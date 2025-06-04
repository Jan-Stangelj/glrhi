#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <unordered_map>
#include <string>

namespace glrhi{
    class shader{
    public:
        shader(const char* vertexPath, const char* fragmentPath);
        ~shader();

        void use();

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
        GLuint m_ID;
        std::unordered_map<std::string_view, GLint> m_cache;

        GLint m_getLocation(const char* name);
    };
}