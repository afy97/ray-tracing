#pragma once

#include "pch.hpp"

#include "Material.hpp"
#include "Shape.hpp"

class Sphere : public Shape {
private:
    glm::vec3 position;
    float radius;

public:
    Sphere(glm::vec3 _position, float _radius, Material _material)
        : Shape(_material), position(_position), radius(_radius)
    {
    }

    glm::vec3 get_position() const { return position; }

    float get_radius() const { return radius; }

    std::tuple<bool, glm::vec3> ray_hit_position(glm::vec3 ray_origin, glm::vec3 ray_direction) override;

    glm::vec3 surface_normal(glm::vec3 surface_point) override;
};
