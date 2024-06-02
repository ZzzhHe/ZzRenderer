#pragma once

#include "Light.hpp"

struct SharedUniform {
    glm::mat4 model;
	std::vector<std::shared_ptr<Light>> lights;
	std::shared_ptr<Texture> shadowMap;
	int cascadeCount;
	std::vector<float> shadowCascadeLevels;
	std::vector<glm::mat4> lightSpaceMatrix;
};

// struct ShadowUniform {
//     glm::mat4 model;
//     glm::mat4 lightSpaceMatrix;
// };

struct ShadowUniform {
    glm::mat4 model;
    std::vector<glm::mat4> lightSpaceMatrices;
};

struct UboCamera {
    glm::mat4 view; // 64
    glm::mat4 projection; // 64
    glm::vec3 viewPos; // 16 (padding 4 to memory alignment)
};
