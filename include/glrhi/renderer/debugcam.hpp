/**
 * @file debugcam.hpp
 * @brief 
 */

#pragma once

#include <glrhi/renderer/camera.hpp>

#include <glrhi/core/window.hpp>

namespace glrhi {
    class debugCamera {
    public:

        debugCamera() = default;
        ~debugCamera() = default;


        /**
         * @brief Updates the refrenced camera's position and direction using input from the supplied window.
         * 
         * @param camera The camera, to which to apply the changes.
         * @param window The window from which to pull input.
         * @param deltaTime Delta time from the previous frame.
         */
        void apply(glrhi::camera& camera, const glrhi::window& window, float deltaTime);

        /**
         * @brief Mouse sensitivity.
         */
        float sensitivity = 0.33f;

        /**
         * @brief The speed at which the camera moves.
         */
        float speed = 1.5f;

    private:

        void m_updateDirection(glrhi::camera& camera, const glrhi::window& window);
        void m_updatePosition(glrhi::camera& camera, const glrhi::window& window, float deltaTime);


        double lastX = 0.0;
        double lastY = 0.0;
        bool firstMouse = true;
    };
}