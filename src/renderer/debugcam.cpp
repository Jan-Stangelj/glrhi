#include "glrhi/renderer/debugcam.hpp"
#include "GLFW/glfw3.h"

namespace glrhi {
    void debugCamera::apply(glrhi::camera& camera, const glrhi::window& window, float deltaTime) {
        m_updateDirection(camera, window);
        m_updatePosition(camera, window, deltaTime);
    }

    void debugCamera::m_updatePosition(glrhi::camera& camera, const glrhi::window& window, float deltaTime) {

        if (glfwGetMouseButton(window.getGlfwWindow(), GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
            glfwSetInputMode(window.getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        }

        glfwSetInputMode(window.getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glm::vec3 forward = glm::vec3(camera.getView()[0][2], camera.getView()[1][2], camera.getView()[2][2]);
        glm::vec3 right = glm::vec3(camera.getView()[0][0], camera.getView()[0][1], camera.getView()[2][0]);
        glm::vec3 up = glm::vec3(camera.getView()[0][1], camera.getView()[1][1], camera.getView()[2][1]);

        if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_W) == GLFW_PRESS)
            camera.position -= forward * speed * deltaTime;
        if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_S) == GLFW_PRESS)
            camera.position += forward * speed * deltaTime;
        if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_A) == GLFW_PRESS)
            camera.position -= right * speed * deltaTime;
        if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_D) == GLFW_PRESS)
            camera.position += right * speed * deltaTime;
        if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_E) == GLFW_PRESS)
            camera.position += up * speed * deltaTime;
        if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_Q) == GLFW_PRESS)
            camera.position -= up * speed * deltaTime;
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

        if (glfwGetMouseButton(window.getGlfwWindow(), GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
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
