#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Data.hpp"

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
		
        for (auto &light : uniformData.lights) {
            light->apply(shader);   
        }

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
