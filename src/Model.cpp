#include "Model.hpp"

#include <iostream>

void Model::Render(Shader* shader) {
    for (unsigned int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i]->Render(shader);
    }
}

Model::Model(const char *path) {
    loadModel(path);
}

Model::~Model() {
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    if (node == nullptr) {
        std::cout << "Node is null" << std::endl;
        return;
    }
    for (unsigned int i = 0; i < node->mNumMeshes; i ++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i ++) {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i ++) {
        Vertex v;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        v.Position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        v.Normal = vector;
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            v.TexCoords = vec;
        } else {
            v.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i ++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j ++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<std::shared_ptr<Texture>> emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, TextureType::EMISSION);
        textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
    }

    return std::make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, TextureType typeName) {
    std::vector<std::shared_ptr<Texture>> textures;
    bool skip;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i ++) {
        aiString str;
        material->GetTexture(type, i, &str);
        skip = false;
        for(unsigned int j = 0; j < m_loaded_textures.size(); j++)
        {
            if(std::strcmp(m_loaded_textures[j]->getFilePath().data(), str.C_Str()) == 0) {
                textures.push_back(m_loaded_textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            auto texture = std::make_shared<Texture>(m_directory + "/" + str.C_Str(), typeName);
            textures.push_back(texture);
            m_loaded_textures.push_back(texture);
        }
    }
    return std::move(textures);
}


