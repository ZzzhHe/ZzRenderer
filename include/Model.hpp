#pragma once

#include <vector>
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public :
    Model(const char *path);
    ~Model();
    void render(const SharedUniform& uniform) const;
    void render(const ShadowUniform& uniform) const;

    using mesh_id_t = unsigned int;
    using mesh_map_t = std::unordered_map<mesh_id_t, std::shared_ptr<Mesh>>;

    void setShader(std::shared_ptr<Shader> shader);
    void setShader(mesh_id_t mesh_id, std::shared_ptr<Shader> shader);
    void setMaterial(std::shared_ptr<Material> material);
    void setMaterial(mesh_id_t mesh_id, std::shared_ptr<Material> material);

private :
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    std::shared_ptr<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType tex_type);

private:
    mesh_map_t m_meshes;
    std::vector<std::shared_ptr<Texture>> m_loaded_textures;
    std::string m_directory;
    static mesh_id_t m_current_id;
};