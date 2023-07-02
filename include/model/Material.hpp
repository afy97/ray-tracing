#pragma once

#include "pch.hpp"

class Material {
private:
    glm::vec3 color;

    float emission_intensity;

public:
    Material(glm::vec3 _color = glm::vec3(1.0f), float _emission_intensity = 0.0f)
        : color(glm::clamp(_color, 0.0f, 1.0f)), emission_intensity(_emission_intensity)
    {
    }

    glm::vec3 get_color() const { return color; }

    float get_emission() const { return emission_intensity; }
};
