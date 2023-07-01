#pragma once

#include "pch.hpp"

class Ray {
private:
    glm::vec3 origin;
    glm::vec3 direction;

public:
    Ray(glm::vec3 _origin, glm::vec3 _direction) : origin(_origin), direction(_direction) {}

    glm::vec3 get_origin() const { return origin; }
    glm::vec3 get_direction() const { return direction; }
};
