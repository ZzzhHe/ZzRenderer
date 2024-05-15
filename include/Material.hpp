#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "utils.hpp"

#include <memory>
#include <vector>

class Material {
public:
    std::shared_ptr<Shader> shader{};
    std::vector<std::shared_ptr<Texture>> textures;

    Material(std::vector<std::shared_ptr<Texture>> textures) 
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
        for (size_t i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
            shader->setInt("texture" + std::to_string(i + 1), i); // temporary
        }
    }

};