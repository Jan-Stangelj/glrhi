#include "glrhi/core/shader.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace glrhi {
    
    shader::shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geomPath) {
        create(vertexPath, fragmentPath, geomPath);
    }

    shader::~shader() {
        if (m_ID)
            glDeleteProgram(m_ID);
    }

    void shader::create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geomPath) {
        if (m_ID) {
            std::cerr << "ERROR::SHADER::SHADER_ALREADY_CREATED\n";
            return;
        }

        std::string vertexCode;
        std::string fragmentCode;
        std::string geomCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            if (!geomPath.empty()) {
                gShaderFile.open(geomPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geomCode = gShaderStream.str();
            }
        } catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        }

        if (!geomCode.empty())
            createFromCode(vertexCode.c_str(), fragmentCode.c_str(), geomCode.c_str());
        else
            createFromCode(vertexCode.c_str(), fragmentCode.c_str(), nullptr);
    }

    void shader::createFromCode(const char* vertexCode, const char* fragmentCode, const char* geomCode) {
        if (m_ID) {
            std::cerr << "ERROR::SHADER::SHADER_ALREADY_CREATED\n";
            return;
        }

        unsigned int vertex, fragment, geometry;
        int success;
        char infoLog[512];

        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCode, nullptr);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        // Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCode, nullptr);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        // Geometry shader (optional)
        if (geomCode) {
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &geomCode, nullptr);
            glCompileShader(geometry);
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
                std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << "\n";
            }
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        if (geomCode)
            glAttachShader(m_ID, geometry);

        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geomCode)
            glDeleteShader(geometry);
    }

    void shader::use() const {
        if (m_ID)
            glUseProgram(m_ID);
    }

    GLint shader::m_getLocation(const char* name) {
        if (m_cache.find(name) != m_cache.end())
            return m_cache[name];

        GLint location = glGetUniformLocation(m_ID, name);
        m_cache[name] = location;
        return location;
    }

    void shader::setBool(const char* name, bool value) { if (m_ID) glProgramUniform1i(m_ID, m_getLocation(name), int(value)); }
    void shader::setInt(const char* name, int value){ if (m_ID) glProgramUniform1i(m_ID, m_getLocation(name), value); }
    void shader::setFloat(const char* name, float value){ if (m_ID) glProgramUniform1f(m_ID, m_getLocation(name), value); }

    void shader::setVec2(const char* name, const glm::vec2 &value){ if (m_ID) glProgramUniform2fv(m_ID, m_getLocation(name), 1, &value[0]); }
    void shader::setVec3(const char* name, const glm::vec3 &value){ if (m_ID) glProgramUniform3fv(m_ID, m_getLocation(name), 1, &value[0]); }
    void shader::setVec4(const char* name, const glm::vec4 &value){ if (m_ID) glProgramUniform4fv(m_ID, m_getLocation(name), 1, &value[0]); }

    void shader::setUVec2(const char* name, const glm::uvec2 &value){ if (m_ID) glProgramUniform2uiv(m_ID, m_getLocation(name), 1, &value[0]); }
    void shader::setUVec3(const char* name, const glm::uvec3 &value){ if (m_ID) glProgramUniform3uiv(m_ID, m_getLocation(name), 1, &value[0]); }
    void shader::setUVec4(const char* name, const glm::uvec4 &value){ if (m_ID) glProgramUniform4uiv(m_ID, m_getLocation(name), 1, &value[0]); }

    void shader::setMat2(const char* name, const glm::mat2 &mat){ if (m_ID) glProgramUniformMatrix2fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]); }
    void shader::setMat3(const char* name, const glm::mat3 &mat){ if (m_ID) glProgramUniformMatrix3fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]); }
    void shader::setMat4(const char* name, const glm::mat4 &mat){ if (m_ID) glProgramUniformMatrix4fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]); }

    void shader::setHandle64(const char* name, const GLuint64& handle){ if (m_ID) glProgramUniformHandleui64ARB(m_ID, m_getLocation(name), handle); }

}
