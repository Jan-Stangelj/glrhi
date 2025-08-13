#pragma once

#include <glrhi/glrhi.hpp>

#include <glm/glm.hpp>

namespace glrhi {
    struct light {
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec4 color;
        float strength;
        float cutoff;
        float outerCutoff;
        int type;
    };
}