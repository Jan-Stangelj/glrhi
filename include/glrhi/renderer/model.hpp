#pragma once

#include <glrhi/glrhi.hpp>
#include <glrhi/renderer/material.hpp>
#include <glrhi/renderer/mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <vector>


namespace glrhi {
    struct submesh {
        glrhi::mesh mesh;
        glrhi::material material;
    };

    class model {
    public:
        model(const std::filesystem::path& path);

        void draw(glrhi::shader& shader);

        glm::vec3 size = glm::vec3(1.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 position = glm::vec3(0.0f);
    private:
        
        std::vector<glrhi::submesh> m_meshes;
        std::filesystem::path directory;

        void processNode(aiNode* node, const aiScene* scene);
        glrhi::submesh processMesh(aiMesh* mesh, const aiScene* scene);
    };
}