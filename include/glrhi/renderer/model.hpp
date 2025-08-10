#pragma once

#include "mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <memory>


namespace Renderer {
    class Model {
    public:
        Model(const std::string& path);

        void draw(Renderer::Shader& shader);

        glm::vec3 size = glm::vec3(1.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 position = glm::vec3(0.0f);
    private:
        
        std::vector<Mesh> meshes;
        std::string directory;

        void processNode(aiNode* node, const aiScene* scene);
        Renderer::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    };
}