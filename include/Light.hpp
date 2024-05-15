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

/*
class PointLight : public Light{
    public:
        PointLight(const glm::vec3& position, const glm::vec3& color)
            : Light(color, 1.0f), position(position), constant(1.0f), linear(0.0014f), quadratic(0.000007f), 
                ambient(color * 0.0f), diffuse(color * 0.7f), specular(color * 1.0f) {}

        void setPosition(const glm::vec3& position) { this->position = position; }

    private:
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
};
*/