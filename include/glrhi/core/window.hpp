/**
 * @file window.hpp
 * @brief Handles creation of windows, swapping buffers and pooling input using GLFW.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace glrhi {

    /**
     * @brief Handles creation of windows, swapping buffers and pooling input using GLFW.
     */
    class window{
    public:

        /**
         * @brief Construct a new window with the provided dimensions and title.
         * 
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         */
        window(unsigned int width, unsigned int height, const char* title);
        window() = default;
        ~window();

        void create(unsigned int width, unsigned int height, const char* title);

        /**
         * @return Returns true if the window should close.
         */
        bool shouldClose() const;

        /**
         * @brief Pools input events.
         */
        void poolEvents() const;

        /**
         * @brief Swaps buffers used for double-buffering.
         */
        void swapBuffers() const;

        /**
         * @brief Returns the GLFW window.
         * 
         * @return GLFWwindow* 
         */
        GLFWwindow* getGlfwWindow() const;

    private:

        GLFWwindow* m_window = nullptr;

    };
}