#include <glrhi/renderer/model.hpp>
#include <iostream>

#include <glm/gtc/quaternion.hpp>
#include <memory>
#include "glm/ext/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace glrhi {
    model::model(const std::filesystem::path& path) {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                        aiProcess_GenNormals |
                                                        aiProcess_OptimizeMeshes |
                                                        aiProcess_OptimizeGraph |
                                                        aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::MODEL::ASSIMP::" << importer.GetErrorString() << "\n";
        }

        m_directory = path.parent_path();

        m_processNode(scene->mRootNode, scene);
    }

    void model::m_processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(m_processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            m_processNode(node->mChildren[i], scene);
        }
    }

    glrhi::submesh model::m_processMesh(aiMesh* mesh, const aiScene* scene) {

        std::vector<vertex> vertices;
        vertices.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glrhi::vertex vertex;

            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            if (mesh->mTextureCoords[0]) {
                vertex.textureUV.x = mesh->mTextureCoords[0][i].x;
                vertex.textureUV.y = mesh->mTextureCoords[0][i].y;
            }
            else {
                vertex.textureUV = glm::vec2(0.0f);
            }

            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;

            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;

            vertices.push_back(vertex);
        }

        std::vector<GLuint> indices;
        indices.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        std::shared_ptr<glrhi::material> material = std::make_shared<glrhi::material>();

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

            aiColor4D albedo;
            float roughness, metallic;

            mat->Get(AI_MATKEY_COLOR_DIFFUSE, albedo);
            mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
            mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic);

            material->albedo = glm::vec4(albedo.r, albedo.g, albedo.b, albedo.a);
            material->arm = glm::vec4(1.0f, roughness, metallic, 0.0f);

            if (mat->GetTextureCount(aiTextureType_DIFFUSE) >= 1) {
                aiString str;
                mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                material->setAlbedoTexture(m_directory.string() + '/' + str.C_Str());
            }
            if (mat->GetTextureCount(aiTextureType_GLTF_METALLIC_ROUGHNESS) >= 1) {
                aiString str;
                mat->GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &str);
                material->setARMtexture(m_directory.string() + '/' + str.C_Str());
            }
            if (mat->GetTextureCount(aiTextureType_NORMALS)) {
                aiString str;
                mat->GetTexture(aiTextureType_NORMALS, 0, &str);
                material->setNormalTexture(m_directory.string() + '/' + str.C_Str());
            }
            if (mat->GetTextureCount(aiTextureType_EMISSIVE) >= 1) {
                aiString str;
                mat->GetTexture(aiTextureType_EMISSIVE, 0, &str);
                material->setEmissionTexture(m_directory.string() + '/' + str.C_Str());
            }
        }

        glrhi::submesh returnMesh;

        returnMesh.material = material;
        returnMesh.mesh = std::make_shared<glrhi::mesh>(vertices, indices);

        return returnMesh;
    }

    void model::draw(glrhi::shader& shader) {
        shader.use();

        glm::mat4 model(1.0f);

        model = glm::scale(model, size);

        glm::quat qPitch = glm::angleAxis(rotation.x, glm::vec3(1, 0, 0));
        glm::quat qYaw   = glm::angleAxis(rotation.y,   glm::vec3(0, 1, 0));
        glm::quat qRoll  = glm::angleAxis(rotation.z,  glm::vec3(0, 0, 1));

        glm::quat orientation = qYaw * qPitch * qRoll;

        model = model * glm::toMat4(orientation);

        model = glm::scale(model, size);

        shader.setMat4("u_model", model);

        for (auto mesh : m_meshes) {
            mesh.material->bind(shader);
            mesh.mesh->draw();
            std::cout << "Draw mesh\n";
        }
    }
}