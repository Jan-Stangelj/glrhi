#include <glrhi/renderer/camera.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace glrhi {
    camera::camera() {
        m_cameraData.create(sizeof(glm::mat4) * 2);
    }

    void camera::uploadData() {
        glm::vec3 dirRad = glm::radians(direction);

        glm::mat4 rotation = glm::eulerAngleXYZ(dirRad.x, dirRad.y, dirRad.z);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotation;
        m_view = glm::inverse(transform);

        m_projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);

        glm::mat4 matrices[2] = {m_view, m_projection};

        m_cameraData.sendData(0, sizeof(glm::mat4) * 2, matrices);
    }

    void camera::bind() const {
        m_cameraData.addBindingPoint(0);
    }
}