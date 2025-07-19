#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string_view>

namespace glrhi{
    class compute{
    public:

        compute(const char* computePath);
        ~compute();

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

        void setHandle64(const char* name, const GLuint64& handle);

    private:
        GLint m_getLocation(const char* name);

        GLuint m_ID;

        std::unordered_map<std::string_view, GLint> m_cache;
    };
}