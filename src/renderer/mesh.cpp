#include <glrhi/renderer/mesh.hpp>

#include <iostream>

namespace glrhi {
    mesh::mesh(const std::vector<vertex>& vertices, const std::vector<GLuint>& indices) {
        load(vertices, indices);
    }

    void mesh::load(const std::vector<vertex>& vertices, const std::vector<GLuint>& indices) {
        if (m_numIndices) {
            std::cerr << "ERROR::RENDERER::MESH::DATA_ALREADY_LOADED\n";
            return;
        }

        m_vbo.create(vertices.data(), vertices.size() * sizeof(vertex));
        m_ebo.create(indices.data(), indices.size() * sizeof(GLuint));
        
        m_vao.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
        m_vao.addAttribute(2, GL_FLOAT, GL_FALSE, offsetof(vertex, textureUV));

        m_vao.init(m_vbo, m_ebo, 5*sizeof(GLfloat));

        m_numIndices = indices.size();
    }

    void mesh::draw() {
        m_vao.bind();
        glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
    }
}