#include "glrhi/core/shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace glrhi {
    
    shader::shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath){
        create(vertexPath, fragmentPath);
    }

    shader::~shader() {
        glDeleteProgram(m_ID);
    }

    void shader::create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
        if (m_ID) {
            std::cerr << "ERROR::SHADER::SHADER_ALREADY_CREATED\n";
            return;
        }

        // retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // read file’s buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        createFromCode(vShaderCode, fShaderCode);
    }

    void shader::createFromCode(const char* vertexCode, const char* fragmentCode) {
        const char* vShaderCode = vertexCode;
        const char* fShaderCode = fragmentCode;

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        // print compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        };

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        // print compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);

        // print linking errors if any
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        }

        // delete shaders; they’re linked into our program and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void shader::use() const {
        glUseProgram(m_ID);
    }

    GLint shader::m_getLocation(const char* name) {
        // if the location is in the cache, return that location
        if (m_cache.find(name) != m_cache.end()) {
            return m_cache[name];
        }

        // Otherwise get the location from OpenGL, add it to the cache and return it
        GLint location = glGetUniformLocation(m_ID, name);
        m_cache[name] = location;
        return location;
    }

    void shader::setBool(const char* name, bool value) {
        glProgramUniform1i(m_ID, m_getLocation(name), (int)value); 
    }
    void shader::setInt(const char* name, int value){ 
        glProgramUniform1i(m_ID, m_getLocation(name), value); 
    }
    void shader::setFloat(const char* name, float value) { 
        glProgramUniform1f(m_ID, m_getLocation(name), value); 
    }

    void shader::setVec2(const char* name, const glm::vec2 &value) { 
        glProgramUniform2fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void shader::setVec3(const char* name, const glm::vec3 &value) { 
        glProgramUniform3fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void shader::setVec4(const char* name, const glm::vec4 &value) { 
        glProgramUniform4fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }

    void shader::setUVec2(const char* name, const glm::uvec2 &value) { 
        glProgramUniform2uiv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void shader::setUVec3(const char* name, const glm::uvec3 &value) { 
        glProgramUniform3uiv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void shader::setUVec4(const char* name, const glm::uvec4 &value) { 
        glProgramUniform4uiv(m_ID, m_getLocation(name), 1, &value[0]); 
    }

    void shader::setMat2(const char* name, const glm::mat2 &mat) {
        glProgramUniformMatrix2fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void shader::setMat3(const char* name, const glm::mat3 &mat) {
        glProgramUniformMatrix3fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void shader::setMat4(const char* name, const glm::mat4 &mat) {
        glProgramUniformMatrix4fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void shader::setHandle64(const char* name, const GLuint64& handle) {
        glProgramUniformHandleui64ARB(m_ID, m_getLocation(name), handle);
    }
}
