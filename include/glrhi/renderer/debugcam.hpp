#pragma once

#include <glrhi/renderer/camera.hpp>

#include <glrhi/core/window.hpp>

namespace glrhi {
    class debugCamera {
    public:

        debugCamera() = default;
        ~debugCamera() = default;

        void apply(glrhi::camera& camera, const glrhi::window& window, float deltaTime);

        float sensitivity = 1.0f;
        float speed = 1.0f;

    private:

        void m_updateDirection(glrhi::camera& camera, const glrhi::window& window);
        void m_updatePosition(glrhi::camera& camera, const glrhi::window& window, float deltaTime);


        double lastX = 0.0;
        double lastY = 0.0;
        bool firstMouse = true;
    };
}