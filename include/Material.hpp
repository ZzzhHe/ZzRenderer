#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

#include <memory>
#include <vector>

class Material {
public:
    std::shared_ptr<Shader> shader{};
    std::vector<std::shared_ptr<Texture>> textures;

    Material(std::vector<std::shared_ptr<Texture>> textures) 
        : textures(textures) {}

    void apply() const {
        if (shader == nullptr) {
            throw std::runtime_error("Shader is nullptr");
        }
        shader->use();
        for (size_t i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
            shader->setInt("texture" + std::to_string(i + 1), i); // temporary
        }
    }

};