#pragma once

#include "Shader.hpp"

enum class LightType {
	Direct,
	Point
};

struct Light {
    glm::vec4 color;
    glm::vec4 ambientColor;
	LightType type;

	Light(glm::vec4 color, glm::vec4 ambientColor, LightType type)
		: color(color), ambientColor(ambientColor), type(type) {}

	virtual void apply(const std::shared_ptr<Shader>& shader, const int index) const = 0;
};

struct DirectLight: public Light{
    glm::vec3 direction;

	DirectLight(glm::vec3 direction, glm::vec4 color, glm::vec4 ambientColor)
		: Light(color, ambientColor, LightType::Direct), direction(direction) {}

	void apply(const std::shared_ptr<Shader>& shader, const int index) const override {
		std::string lightwithindex = "directLight[" + std::to_string(index) + "]";
		shader->setVec3(lightwithindex + ".direction", direction);
		shader->setVec4(lightwithindex + ".ambientColor", ambientColor);
		shader->setVec4(lightwithindex + ".color", color);
	}
};

struct PointLight: public Light{
	glm::vec3 position;
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	LightType type = LightType::Point;

	PointLight(glm::vec3 position, glm::vec4 color, glm::vec4 ambientColor)
		: Light(color, ambientColor, LightType::Point), position(position) {}
	PointLight(glm::vec3 position, glm::vec4 color, glm::vec4 ambientColor, float constant, float linear, float quadratic)
		: Light(color, ambientColor, LightType::Point), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

	void apply(const std::shared_ptr<Shader>& shader, const int index) const override {
		std::string lightwithindex = "pointLight[" + std::to_string(index) + "]";
		shader->setVec3(lightwithindex+ ".position", position);
		shader->setVec4(lightwithindex + ".ambientColor", ambientColor);
		shader->setVec4(lightwithindex + ".color", color);
		shader->setFloat(lightwithindex + ".constant", constant);
		shader->setFloat(lightwithindex + ".linear", linear);
		shader->setFloat(lightwithindex + ".quadratic", quadratic);
	}

};
