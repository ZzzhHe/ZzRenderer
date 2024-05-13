#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

#include <memory>
#include <vector>

class Material {
public:
    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Texture>> textures;

    void bind() const {
        shader->use();
        for (size_t i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
        }
    }

};