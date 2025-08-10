/**
 * @file mesh.hpp
 * @brief Handles the loading and drawing of meshes.
 */

#pragma once

#include <glrhi/glrhi.hpp>

#include <glm/glm.hpp>
#include <vector>

namespace glrhi {

    struct vertex {
        glm::vec3 position;
        glm::vec2 textureUV;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    /**
     * @brief Handles the loading and drawing of meshes.
     */
    class mesh{
    public:

        /**
         * @brief Construct a new mesh and loads the provided data to GPU memory.
         * 
         * @param vertices Refrence to the vertices to be loaded.
         * @param indices Refrence to the indices to be loaded.
         */
        mesh(const std::vector<vertex>& vertices, const std::vector<GLuint>& indices);
        mesh() = default;
        ~mesh() = default;

        /**
         * @brief Loads the provided data to gpu memory.
         * 
         * @param vertices Refrence to the vertices to be loaded.
         * @param indices Refrence to the indices to be loaded.
         */
        void load(const std::vector<vertex>& vertices, const std::vector<GLuint>& indices);

        /**
         * @brief Issues a draw call with the appropriate number of indices.
         */
        void draw();

    private:

        glrhi::vbo m_vbo;
        glrhi::vao m_vao;
        glrhi::ebo m_ebo;

        int m_numIndices = 0;

    };
}