#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <string>

namespace glrhi {
    class window{
    public:

        window(unsigned int width, unsigned int height, const char* title);
        ~window();

        bool shouldClose() const;

        void swapBuffers() const;

        GLFWwindow* getGlfwWindow() const;

    private:

        GLFWwindow* m_window;

    };
}