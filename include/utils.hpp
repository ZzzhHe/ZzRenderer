#pragma once

#include "Light.hpp"

struct SharedUniform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
	DirectLight directLight;
    PointLight pointLight;
	glm::vec3 viewPos;
};
