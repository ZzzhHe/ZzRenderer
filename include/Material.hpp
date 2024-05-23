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
		
		shader->setVec3("directLight.direction", uniformData.directLight.direction);
		shader->setVec4("directLight.ambientColor", uniformData.directLight.ambientColor);
		shader->setVec4("directLight.color", uniformData.directLight.color);
		
		shader->setVec3("pointLight.position", uniformData.pointLight.position);
		shader->setVec4("pointLight.ambientColor", uniformData.pointLight.ambientColor);
		shader->setVec4("pointLight.color", uniformData.pointLight.color);
		shader->setFloat("pointLight.constant", uniformData.pointLight.constant);
		shader->setFloat("pointLight.linear", uniformData.pointLight.linear);
		shader->setFloat("pointLight.quadratic", uniformData.pointLight.quadratic);

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
