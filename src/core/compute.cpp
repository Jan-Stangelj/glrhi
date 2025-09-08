#include "glrhi/core/compute.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace glrhi {
    
    compute::compute(const std::filesystem::path& computePath){
        create(computePath);
    }

    compute::~compute() {
        if (m_ID)
            glDeleteProgram(m_ID);
    }

    void compute::create(const std::filesystem::path& computePath) {

        if (m_ID) {
            std::cerr << "ERROR::SHADER::COMPUTE::SHADER_ALREADY_CREATED\n";
            return;
        }

        std::string computeCode;
        std::ifstream cShaderFile;

        // Enables warnings when reading files.
        cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            cShaderFile.open(computePath);

            // Reads the file into a temporary variable
            std::stringstream cShaderStream;
            cShaderStream << cShaderFile.rdbuf();

            cShaderFile.close();

            computeCode = cShaderStream.str(); // Copies the temp variable
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::COMPUTE::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* cShaderCode = computeCode.c_str();

        createFromCode(cShaderCode);
    }

    void compute::createFromCode(const char* code) {
        GLint success;
        GLchar infoLog[1024];

        GLuint compute;


        // Compiles the shader
        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &code, NULL);
        glCompileShader(compute);

        glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(compute, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << "\n";
        }
        
        // Creates a program and links the compiled shader to it 
        m_ID = glCreateProgram();
        glAttachShader(m_ID, compute);
        glLinkProgram(m_ID);
        
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(m_ID, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPUTE::LINKING_FAILED\n" << infoLog << "\n";
        }

        // Deletes the shader, beacouse it's no longer used.
        glDeleteShader(compute);
    }

    void compute::dispatch(GLuint x, GLuint y, GLuint z) const {
        if (!m_ID)
            return;
        glUseProgram(m_ID);
        glDispatchCompute(x, y, z);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);   // Wait until shader finishes execution
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
        if (m_ID) glProgramUniform1i(m_ID, m_getLocation(name), (int)value); 
    }
    void compute::setInt(const char* name, int value){ 
        if (m_ID) glProgramUniform1i(m_ID, m_getLocation(name), value); 
    }
    void compute::setFloat(const char* name, float value) { 
        if (m_ID) glProgramUniform1f(m_ID, m_getLocation(name), value); 
    }

    void compute::setVec2(const char* name, const glm::vec2 &value) { 
        if (m_ID) glProgramUniform2fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setVec3(const char* name, const glm::vec3 &value) { 
        if (m_ID) glProgramUniform3fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setVec4(const char* name, const glm::vec4 &value) { 
        if (m_ID) glProgramUniform4fv(m_ID, m_getLocation(name), 1, &value[0]); 
    }

    void compute::setUVec2(const char* name, const glm::uvec2 &value) { 
        if (m_ID) glProgramUniform2uiv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setUVec3(const char* name, const glm::uvec3 &value) { 
        if (m_ID) glProgramUniform3uiv(m_ID, m_getLocation(name), 1, &value[0]); 
    }
    void compute::setUVec4(const char* name, const glm::uvec4 &value) { 
        if (m_ID) glProgramUniform4uiv(m_ID, m_getLocation(name), 1, &value[0]); 
    }

    void compute::setMat2(const char* name, const glm::mat2 &mat) {
        if (m_ID) glProgramUniformMatrix2fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void compute::setMat3(const char* name, const glm::mat3 &mat) {
        if (m_ID) glProgramUniformMatrix3fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void compute::setMat4(const char* name, const glm::mat4 &mat) {
        if (m_ID) glProgramUniformMatrix4fv(m_ID, m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void compute::setHandle64(const char* name, const GLuint64& handle) {
        if (m_ID) glProgramUniformHandleui64ARB(m_ID, m_getLocation(name), handle);
    }
}
