#include "glrhi/renderer/debugcam.hpp"
#include "GLFW/glfw3.h"

namespace glrhi {
    void debugCamera::apply(glrhi::camera& camera, const glrhi::window& window, float deltaTime) {
        m_updateDirection(camera, window);
        //m_updatePosition(camera, window, deltaTime);
    }

    void debugCamera::m_updateDirection(glrhi::camera& camera, const glrhi::window& window) {
        double x, y;

        glfwGetCursorPos(window.getGlfwWindow(), &x, &y);

        if (firstMouse) {
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        double xoffset = lastX - x;
        double yoffset = lastY - y;

        lastX = x;
        lastY = y;

        if (glfwGetMouseButton(window.getGlfwWindow(), GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
            return;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.direction.y += xoffset;
        camera.direction.x += yoffset;

        if (camera.direction.x > 89.0f)
            camera.direction.x = 89.0f;
        if (camera.direction.x < -89.0f)
            camera.direction.x = -89;
    }
}
