// src/include/glrhi/core/vao.hpp
/**
 * @file vao.hpp
 * @brief OpenGL Vertex Array Object class definition.
 */

#pragma once

#include <glad/glad.h>

#include "glrhi/core/vbo.hpp"
#include "glrhi/core/ebo.hpp"

#include <vector>

namespace glrhi {
    /**
     * @brief Represents an OpenGL Vertex Array Object (VAO).
     */
    class vao {
    public:

        /**
         * @brief Constructs a VAO.
         */
        vao();
        /**
         * @brief Destroys the VAO.
         */
        ~vao();

        /**
         * @brief Adds a vertex attribute to the VAO.
         * @param size The size of the vertex attribute.
         * @param type The data type of the vertex attribute.
         * @param normalized Whether the vertex attribute is normalized.
         * @param pointer The pointer to the vertex attribute data.
         */
        void addAttribute(GLint size, GLenum type, GLboolean normalized, GLuint pointer);

        /**
         * @brief Initializes the VAO with a VBO and EBO.
         * @param VBO The vertex buffer object.
         * @param EBO The element buffer object.
         * @param stride The stride of the vertex data.
         */
        void init(const glrhi::vbo& VBO, const glrhi::ebo& EBO, GLsizei stride) const;

        /**
         * @brief Binds the VAO.
         */
        void bind() const;

        /**
         * @brief Unbinds the VAO.
         */
        void unbind() const;


        /**
         * @brief Gets the VAO ID.
         * @return The VAO ID.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID;

        unsigned int m_attributeCounter = 0;
    };
}