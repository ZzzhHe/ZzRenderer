#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Data.hpp"

#include <memory>
#include <vector>

struct MaterialTextures {
    std::shared_ptr<Texture> albedo;
    std::shared_ptr<Texture> metallic;
	std::shared_ptr<Texture> roughness;
	std::shared_ptr<Texture> normal;
	std::shared_ptr<Texture> ao;
	
	std::shared_ptr<Texture> emission;
	std::shared_ptr<Texture> shadow;
	
    bool operator==(const MaterialTextures& other) const {
        return albedo == other.albedo
			&& metallic == metallic
            && roughness == other.roughness
			&& normal == other.normal
			&& ao == other.ao
			&& emission == other.emission
            && shadow == other.shadow;
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

        if (textures->albedo) {
            shader->setInt("material.albedo", 0);
            textures->albedo->bind(0);
        }
        if (textures->metallic) {
            shader->setInt("material.metallic", 1);
            textures->metallic->bind(1);
        }
		
		if (textures->roughness) {
			shader->setInt("material.roughness", 2);
			textures->roughness->bind(2);
		}
		
		if (textures->normal) {
			shader->setInt("material.normal", 3);
			textures->normal->bind(3);
		}
		
		if (textures->ao) {
			shader->setInt("material.ao", 4);
			textures->ao->bind(4);
		}
		
		if (textures->emission) {
			shader->setInt("material.emission", 5);
			textures->emission->bind(5);
			shader->setBool("hasEmissionMap", true);
		} else {
			shader->setBool("hasEmissionMap", false);
		}

		
		textures->shadow = uniformData.shadowMap;
		
		if (textures->shadow) {
			shader->setInt("shadowMap", 6);
			textures->shadow->bind(6);
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
