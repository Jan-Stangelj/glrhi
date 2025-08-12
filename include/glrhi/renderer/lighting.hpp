#pragma once

#include <glrhi/glrhi.hpp>

#include <glm/glm.hpp>

namespace glrhi {
    enum lightType {
        POINT,
        DIRECTIONAL,
        SPOT
    };
    struct light {
        glm::vec3 position;
        glm::vec3 direction;

        glm::vec3 color;
        float strength;

        float cutoff;
        float outerCutoff;

        lightType type;
    };
}