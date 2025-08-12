#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include <glrhi/renderer/camera.hpp>

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace glrhi {
    camera::camera() {
        m_cameraData.create(sizeof(glm::mat4) * 2 + sizeof(glm::vec4));
    }

    void camera::uploadData() {
        glm::vec3 dirRad = glm::radians(direction);

        glm::quat qPitch = glm::angleAxis(dirRad.x, glm::vec3(1, 0, 0));
        glm::quat qYaw   = glm::angleAxis(dirRad.y,   glm::vec3(0, 1, 0));
        glm::quat qRoll  = glm::angleAxis(dirRad.z,  glm::vec3(0, 0, 1));

        glm::quat orientation = qYaw * qPitch * qRoll;

        glm::mat4 rotation = glm::toMat4(orientation);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotation;
        m_view = glm::inverse(transform);

        m_projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);

        glm::mat4 matrices[2] = {m_view, m_projection};

        m_cameraData.sendData(0, sizeof(glm::mat4) * 2, matrices);
        m_cameraData.sendData(sizeof(glm::mat4) * 2, sizeof(glm::vec4), &position);
    }

    void camera::bind() const {
        m_cameraData.addBindingPoint(0);
    }
}