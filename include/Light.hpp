#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec4 color;
    glm::vec4 ambientColor;
};

struct DirectLight {
    glm::vec3 direction;
    glm::vec4 color;
    glm::vec4 ambientColor;
};

struct PointLight {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec4 ambientColor;
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};
