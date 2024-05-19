#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "utils.hpp"

#include <memory>
#include <vector>

struct MaterialTextures {
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    std::shared_ptr<Texture> normal;

    bool operator==(const MaterialTextures& other) const {
        return diffuse == other.diffuse 
            && specular == other.specular 
            && normal == other.normal;
    }
};

class Material {
public:
    std::shared_ptr<Shader> shader{};
    std::shared_ptr<MaterialTextures> textures;

    Material(const std::shared_ptr<MaterialTextures>& textures) 
        : textures(textures) {}

    void apply(const SharedUniform& uniformData) const {
        if (shader == nullptr) {
            throw std::runtime_error("Shader is nullptr");
        }
        shader->use();
        shader->setMat4("model", uniformData.model);
        shader->setMat4("view", uniformData.view);
        shader->setMat4("projection", uniformData.projection);
        shader->setVec3("directLight.direction", uniformData.light.direction);
        shader->setVec4("directLight.ambientColor", uniformData.light.ambientColor);
        shader->setVec4("directLight.color", uniformData.light.color);
		shader->setVec3("viewPos", uniformData.viewPos);
        if (textures->diffuse) {
            shader->setInt("material.diffuse", 0);
            textures->diffuse->bind(0);
        }
        if (textures->specular) {
            shader->setInt("material.specular", 1);
            textures->specular->bind(1);
        }
        if (textures->normal) {
            shader->setInt("material.normal", 2);
            textures->normal->bind(2);
        }
    }
};
