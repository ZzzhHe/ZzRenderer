#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Data.hpp"

#include <memory>
#include <vector>

struct MaterialTextures {
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
	std::shared_ptr<Texture> emission;
    std::shared_ptr<Texture> normal;
	std::shared_ptr<Texture> shadow;
	
    bool operator==(const MaterialTextures& other) const {
        return diffuse == other.diffuse 
            && specular == other.specular 
			&& emission == other.emission
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
		shader->setMat4Array("lightSpaceMatrices", uniformData.lightSpaceMatrix);
		shader->setInt("cascadeCount", uniformData.cascadeCount);
		shader->setFloatArray("cascadePlaneDistances", uniformData.shadowCascadeLevels);
        
        int pointLightCount = 0;
        int directLightCount = 0;

        for (auto &light : uniformData.lights) {
            if (light->type == LightType::Point) {
                light->apply(shader, pointLightCount++);
            } else if (light->type == LightType::Direct) {
                light->apply(shader, directLightCount++);
            }
        }

        shader->setInt("numPointLights", pointLightCount);
        shader->setInt("numDirectLights", directLightCount);

        if (textures->diffuse) {
            shader->setInt("material.diffuse", 0);
            textures->diffuse->bind(0);
        }
        if (textures->specular) {
            shader->setInt("material.specular", 1);
            textures->specular->bind(1);
        }
		if (textures->emission) {
			shader->setInt("material.emission", 2);
			textures->emission->bind(2);
			shader->setBool("hasEmissionMap", true);
		} else {
			shader->setBool("hasEmissionMap", false);
		}
        if (textures->normal) {
            shader->setInt("material.normal", 3);
            textures->normal->bind(3);
        }
		
		textures->shadow = uniformData.shadowMap;
		
		if (textures->shadow) {
			shader->setInt("shadowMap", 4);
			textures->shadow->bind(4);
		}
    }

    void apply(const ShadowUniform& uniformData) const {
        if (shader == nullptr) {
            throw std::runtime_error("Shader is nullptr");
        }
        shader->use();
        shader->setMat4("model", uniformData.model);
        shader->setMat4Array("lightSpaceMatrices", uniformData.lightSpaceMatrices);
    };
};
