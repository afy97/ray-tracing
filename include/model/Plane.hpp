#pragma once

#include "Shape.hpp"

class Plane : public Shape {
private:
    glm::vec3 normal;

public:
    Plane(Material _material, glm::vec3 _position, glm::vec3 _normal)
        : Shape(_material, _position), normal(glm::normalize(_normal))
    {
    }

    std::tuple<bool, glm::vec3> ray_hit_position(Ray ray) override;

    glm::vec3 surface_normal(glm::vec3 surface_point) override { return normal; }
};
