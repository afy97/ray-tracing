#pragma once

#include "pch.hpp"

class Material {
private:
    glm::vec3 color;
    
    float emmission_intensity;

public:
    Material(glm::vec3 _color = glm::vec3(1.0f), float _emmission_intensity = 0.0f)
        : color(glm::normalize(_color)), emmission_intensity(_emmission_intensity)
    {
    }

    glm::vec3 get_color() const { return color; }
    glm::vec3 get_emmission() const { return emmission_intensity * color; }
};
