#include "Model.hpp"

#include <iostream>

// static member initialization, must be done outside of the class
Model::mesh_id_t Model::m_current_id = 0;

void Model::render(SharedUniform& uniform) const {
	uniform.model = m_modelMatrix;
    for (auto &kv : m_meshes) {
        kv.second->render(uniform);
    }
}

void Model::render(ShadowUniform& uniform) const {
	uniform.model = m_modelMatrix;
    for (auto &kv : m_meshes) {
        kv.second->render(uniform);
    }
}

Model::Model(const char *path) {
    loadModel(path);
    m_current_id = 0;
}

Model::~Model() {
}

void Model::setShader(std::shared_ptr<Shader> shader) {
    for (auto &kv : m_meshes) {
        kv.second->setShader(shader);
    }
}

void Model::setShader(mesh_id_t mesh_id, std::shared_ptr<Shader> shader) {
    auto it = m_meshes.find(mesh_id);
    if (it == m_meshes.end()) {
        throw std::runtime_error("Mesh id not found");
    }
    m_meshes[mesh_id]->setShader(shader);
}

void Model::setMaterial(std::shared_ptr<Material> material) {
    for (auto &kv : m_meshes) {
        kv.second->setMaterial(material);
    }
}

void Model::setMaterial(mesh_id_t mesh_id, std::shared_ptr<Material> material) {
    auto it = m_meshes.find(mesh_id);
    if (it == m_meshes.end()) {
        throw std::runtime_error("Mesh id not found");
    }
    m_meshes[mesh_id]->setMaterial(material);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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
        m_meshes[m_current_id] = processMesh(mesh, scene);
        m_current_id ++;
    }
    for (unsigned int i = 0; i < node->mNumChildren; i ++) {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

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
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        v.Tangent = vector;
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        v.Bitangent = vector;
        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i ++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j ++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::shared_ptr<MaterialTextures> materialTextures = std::make_shared<class MaterialTextures>();
	
	materialTextures->albedo = nullptr;
	
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        materialTextures->albedo = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::ALBEDO);
		// cannot be nullptr?
        materialTextures->metallic = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::METALLIC);
		materialTextures->roughness = loadMaterialTextures(material, aiTextureType_SHININESS, TextureType::ROUGHNESS);
        materialTextures->normal = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::NORMAL);
		materialTextures->ao = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::AO);
		
		materialTextures->emission = loadMaterialTextures(material, aiTextureType_EMISSIVE, TextureType::EMISSION);
    }


    auto material = std::make_shared<Material>(materialTextures);

    return std::make_shared<Mesh>(vertices, indices, material);
}

std::shared_ptr<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, TextureType typeName) {
    // TODO: right now we only load the first texture of each type
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        std::string texturePath = str.C_Str();

        // Check if texture is already loaded
        for (auto& loadedTexture : m_loaded_textures) {
            if (loadedTexture->getFilePath() == texturePath) {
                return loadedTexture; // Return the existing texture if already loaded
            }
        }

        // If texture is not loaded, load it, store in the cache, and return it
        auto texture = std::make_shared<Texture>(m_directory + "/" + texturePath, typeName);
        m_loaded_textures.push_back(texture);
        return texture; // Return the new texture after loading it
    }

    return nullptr; // Return nullptr if no textures are found
}

void Model::setModelMatrix(const glm::mat4 &modelMatrix) {
	m_modelMatrix = modelMatrix;
}


