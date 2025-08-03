/**
 * @file camera.hpp
 * @brief Generic camera class.
 */

#pragma once

#include <glm/glm.hpp>
#include <glrhi/glrhi.hpp>

namespace glrhi {

    /**
     * @brief Generic camera class.
     */
    class camera {
    public:

        camera();
        ~camera() = default;

        /**
         * @brief Recalculates the matrices and uploads them to the GPU.
         */
        void uploadData() const;
        
        /**
         * @brief Binds the UBO with the data to binding point 0.
         */
        void bind() const;

        /**
         * @brief Position of the camera.
         */
        glm::vec3 position = glm::vec3(0.0f);

        /**
         * @brief Euler direction of the camera in degrees. (pitch, yaw, roll)
         */
        glm::vec3 direction = glm::vec3(0.0f);

        /**
         * @brief Distance to the near plane.
         */
        float near = 0.1f;

        /**
         * @brief Distance to the far plane.
         */
        float far = 100.0f;

        /**
         * @brief Fov in the y dimension.
         */
        float fov = 60.0f;

        /**
         * @brief width / height
         */
        float aspectRatio = 16.0f / 9.0f;

    private:

        glrhi::ubo m_cameraData;

    };
}