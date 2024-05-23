#pragma once

#include "Light.hpp"

struct SharedUniform {
    glm::mat4 model;
	DirectLight directLight;
    PointLight pointLight;
};

struct UboCamera {
    glm::mat4 view; // 64
    glm::mat4 projection; // 64
    glm::vec3 viewPos; // 16 (padding 4 to memory alignment)
};