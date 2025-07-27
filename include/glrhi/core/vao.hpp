/**
 * @file vao.hpp
 * @brief Handles the creation and usage of Vertex Array Objects.
 */

#pragma once

#include <glad/glad.h>

#include "glrhi/core/vbo.hpp"
#include "glrhi/core/ebo.hpp"

namespace glrhi {

    /**
     * @brief Handles the creation and usage of Vertex Array Objects.
     */
    class vao {
    public:

        /**
         * @brief Construct a new VAO.
         */
        vao();
        ~vao();

        /**
         * @brief Adds an attribute to the VAO.
         * 
         * @param size Size of the attribute (not in bytes, count, see triangle example).
         * @param dataType Data type of the attribute.
         * @param normalized True if the attribute is normalized.
         * @param offset The offset from previous attributes.
         */
        void addAttribute(GLint size, GLenum dataType, GLboolean normalized, GLuint offset);

        /**
         * @brief Inits the VAO with the added attributes and links 
         *        the VBO and EBO to it.
         * 
         * @param VBO The VBO to link to the VAO.
         * @param EBO The EBO to link to the VAO.
         * @param stride The size of all attributes (not in bytes, count, see triangle example).
         */
        void init(const glrhi::vbo& VBO, const glrhi::ebo& EBO, GLsizei stride) const;

        /**
         * @brief Binds the VAO for drawind.
         */
        void bind() const;

        /**
         * @brief Unbinds the VAO, ie. binds VAO 0.
         */
        void unbind() const;

        /**
         * @brief Returns the VAO's OpenGL ID.
         * 
         * @return The VAO's OpenGL ID.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID = 0;

        unsigned int m_attributeCounter = 0;
    };
}