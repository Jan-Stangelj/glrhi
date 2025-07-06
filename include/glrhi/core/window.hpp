/**
 * @file window.hpp
 * @brief Defines the `glrhi::window` class.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <string>

namespace glrhi {
    
    /**
     * @brief Represents a GLFW window.
     *
     * This class handles the creation, management, and destruction of a GLFW window.  It also enables OpenGL debug output.
     */
    class window{
    public:

        /**
         * @brief Constructs a new `window` object.
         *
         * @param width The width of the window in pixels.
         * @param height The height of the window in pixels.
         * @param title The title of the window.
         */
        window(unsigned int width, unsigned int height, const char* title);

        /**
         * @brief Destroys the `window` object, terminating GLFW.
         */
        ~window();

        /**
         * @brief Checks if the window should close.
         *
         * @return True if the window should close, false otherwise.
         */
        bool shouldClose() const;

        /**
         * @brief Swaps the front and back buffers of the window.
         *
         * This also processes any pending GLFW events.
         */
        void swapBuffers() const;

        /**
         * @brief Returns the GLFW window pointer.
         *
         * @return The GLFW window pointer.
         */
        GLFWwindow* getGlfwWindow() const;

    private:

        GLFWwindow* m_window;

    };
}