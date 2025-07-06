/**
 * @file ebo.hpp
 * @brief Defines the element buffer object (EBO) class.
 */

#pragma once

#include <glad/glad.h>

namespace glrhi {

    /**
     * @brief Represents an element buffer object (EBO).
     */
    class ebo {
    public:

        /**
        * @brief Constructs an EBO with the given data and size.
        * @param data A pointer to the element data.
        * @param size The size of the element data in bytes.
        */
        ebo(const void* data, unsigned int size);

        /**
         * @brief Destructor for the EBO.
         */
        ~ebo();

        /**
         * @brief Binds the EBO.
         */
        void bind() const;

        /**
         * @brief Unbinds the EBO.
         */
        void unbind() const;
        
        /**
         * @brief Returns the ID of the EBO.
         * @return The ID of the EBO.
         */
        GLuint getID() const { return m_ID; }

    private:

        GLuint m_ID;

    };
}
