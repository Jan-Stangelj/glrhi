/**
 * @file vbo.hpp
 * @brief Defines the VBO (Vertex Buffer Object) class.
 */

#pragma once

#include <glad/glad.h>

namespace glrhi {

    /**
     * @brief Represents a Vertex Buffer Object.
     */
    class vbo {
    public:

        /**
         * @brief Constructs a VBO.
         * @param data Pointer to the vertex data.
         * @param size Size of the vertex data in bytes.
         */
        vbo(const void* data, unsigned int size);

        /**
         * @brief Destructor for the VBO.
         */
        ~vbo();

        /**
         * @brief Binds the VBO.
         */
        void bind() const;

        /**
         * @brief Unbinds the VBO.
         */
        void unbind() const;

        /**
         * @brief Returns the ID of the VBO.
         * @return The ID of the VBO.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID;
        
    };
}