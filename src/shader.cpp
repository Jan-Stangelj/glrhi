#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

glrhi::shader::shader(const char* vertexPath, const char* fragmentPath){

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
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

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

    glUseProgram(m_ID);
    
}

glrhi::shader::~shader() {
    glDeleteProgram(m_ID);
}

void glrhi::shader::use(){
    glUseProgram(m_ID);
}

GLint glrhi::shader::m_getLocation(const char* name) {
    if (m_cache.find(name) != m_cache.end())
        return m_cache[name];
    GLint location = glGetUniformLocation(m_ID, name);
    m_cache[name] = location;
    return location;
}

void glrhi::shader::setBool(const char* name, bool value) {
    glUniform1i(m_getLocation(name), (int)value); 
}
void glrhi::shader::setInt(const char* name, int value){ 
    glUniform1i(m_getLocation(name), value); 
}
void glrhi::shader::setFloat(const char* name, float value) { 
    glUniform1f(m_getLocation(name), value); 
}
void glrhi::shader::setVec2(const char* name, const glm::vec2 &value) { 
    glUniform2fv(m_getLocation(name), 1, &value[0]); 
}
void glrhi::shader::setVec3(const char* name, const glm::vec3 &value) { 
    glUniform3fv(m_getLocation(name), 1, &value[0]); 
}
void glrhi::shader::setVec4(const char* name, const glm::vec4 &value) { 
    glUniform4fv(m_getLocation(name), 1, &value[0]); 
}
void glrhi::shader::setMat2(const char* name, const glm::mat2 &mat) {
    glUniformMatrix2fv(m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
}
void glrhi::shader::setMat3(const char* name, const glm::mat3 &mat) {
    glUniformMatrix3fv(m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
}
void glrhi::shader::setMat4(const char* name, const glm::mat4 &mat) {
    glUniformMatrix4fv(m_getLocation(name), 1, GL_FALSE, &mat[0][0]);
}