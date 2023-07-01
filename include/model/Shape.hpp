#pragma once

#include "pch.hpp"

#include "Material.hpp"

class Shape {
private:
    Material material;

    glm::vec3 position;

public:
    Shape(Material _material, glm::vec3 _position) : material(_material), position(_position) {}

    Material get_material() const { return material; }

    glm::vec3 get_position() const { return position; }

    virtual std::tuple<bool, glm::vec3> ray_hit_position(glm::vec3 ray_origin, glm::vec3 ray_direction) = 0;

    virtual glm::vec3 surface_normal(glm::vec3 surface_point) = 0;
};
