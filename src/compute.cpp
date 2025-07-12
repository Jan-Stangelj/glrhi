#include "glrhi/core/compute.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace glrhi {
    
    compute::compute(const char* computePath){

        std::string computeCode;
        std::ifstream cShaderFile;

        cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            cShaderFile.open(computePath);

            std::stringstream cShaderStream;

            cShaderStream << cShaderFile.rdbuf();

            cShaderFile.close();

            computeCode = cShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* cShaderCode = computeCode.c_str();

        GLint success;
        GLchar infoLog[1024];

        unsigned int compute;

        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &cShaderCode, NULL);
        glCompileShader(compute);

        glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(compute, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << "\n";
        }
        
        // shader Program
        m_ID = glCreateProgram();
        glAttachShader(m_ID, compute);
        glLinkProgram(m_ID);
        
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(m_ID, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPUTE::LINKING_FAILED\n" << infoLog << "\n";
        }

        glDeleteShader(compute);
    }

    compute::~compute() {
        glDeleteProgram(m_ID);
    }

    void compute::dispatch(GLuint x, GLuint y, GLuint z) const {
        glUseProgram(m_ID);
        glDispatchCompute(x, y, z);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    GLint compute::m_getLocation(const char* name) {
        // if the location is in the cache, return that location
        if (m_cache.find(name) != m_cache.end()) {
            return m_cache[name];
        }

        // Otherwise get the location from OpenGL, add it to the cache and return it
        GLint location = glGetUniformLocation(m_ID, name);
        m_cache[name] = location;
        return location;
    }

    void compute::setBool(const char* name, bool value) {
        glProgramUniform1i(m_ID, m_getLocation(name), (int)value); 
    }
    void compute::setInt(const char* name, int value){ 
        glProgramUniform1i(m_ID, m_getLocation(name), value); 
    }
    void compute::setFloat(const char* name, float value) { 
        glProgramUniform1f(m_ID, m_getLocation(name), value); 
    }
    void compute::setVec2(const char* name, const glm::vec2 &value) { 
        glProgramUniform2fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setVec3(const char* name, const glm::vec3 &value) { 
        glProgramUniform3fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setVec4(const char* name, const glm::vec4 &value) { 
        glProgramUniform4fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setMat2(const char* name, const glm::mat2 &mat) {
        glProgramUniformMatrix2fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void compute::setMat3(const char* name, const glm::mat3 &mat) {
        glProgramUniformMatrix3fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void compute::setMat4(const char* name, const glm::mat4 &mat) {
        glProgramUniformMatrix4fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
}
