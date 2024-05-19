#pragma once

#include "Light.hpp"

struct SharedUniform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    DirectLight light;
	glm::vec3 viewPos;
};
