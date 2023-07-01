#pragma once

#include "Shape.hpp"

class Sphere : public Shape {
private:
    float radius;

public:
    Sphere(Material _material, glm::vec3 _position, float _radius) : Shape(_material, _position), radius(_radius) {}

    float get_radius() const { return radius; }

    std::tuple<bool, glm::vec3> ray_hit_position(Ray ray) override;

    glm::vec3 surface_normal(glm::vec3 surface_point) override;
};
