/**
 * @file model.hpp
 * @brief Contains the model class for loading 3d models.
 */

#pragma once

#include "assimp/material.h"
#include "assimp/mesh.h"
#include <glrhi/glrhi.hpp>
#include <glrhi/renderer/material.hpp>
#include <glrhi/renderer/mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <vector>
#include <memory>


namespace glrhi {

    /**
     * @brief Internal format, used in the model class.
     */
    struct submesh {
        std::unique_ptr<glrhi::mesh> mesh;
        std::unique_ptr<glrhi::material> material;
    };

    /**
     * @brief Class used to load 3d models.
     */
    class model {
    public:

        /**
         * @brief Construct a new model object and loads the provided 3d model.
         * 
         * @param path Path to the 3d model.
         */
        model(const std::filesystem::path& path);
        model() = default;
        ~model() = default;

        /**
         * @brief Loads a 3d model, if not already.
         * 
         * @param path Path to the 3d model.
         */
        void create(const std::filesystem::path& path);

        /**
         * @brief Issues a draw call for all sub meshes.
         * 
         * @param shader The shader to render the model with.
         */
        void draw(glrhi::shader& shader) const;

        /**
         * @brief Size of the model.
         */
        glm::vec3 size = glm::vec3(1.0f);

        /**
         * @brief Rotation of the model.
         */
        glm::vec3 rotation = glm::vec3(0.0f);

        /**
         * @brief Position of the model.
         */
        glm::vec3 position = glm::vec3(0.0f);
    private:
        
        std::vector<glrhi::submesh> m_meshes;
        std::filesystem::path m_directory;

        void m_processNode(aiNode* node, const aiScene* scene);
        void m_processMesh(aiMesh* mesh, const aiScene* scene);

        void m_processVertices(aiMesh* mesh, std::vector<vertex>& verticesOutput);
        void m_processIndices(aiMesh* mesh, std::vector<GLuint>& indicesOutput);
        void m_optimizeMesh(std::vector<vertex>& verticesInOut, std::vector<GLuint>& indicesInOut);
        void m_processMaterial(aiMaterial* mesh, std::unique_ptr<glrhi::material>& materialOutput);

        glm::mat4 m_calcModelMatrix() const;
    };
}