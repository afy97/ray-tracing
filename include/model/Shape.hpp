#pragma once

#include "pch.hpp"

#include "Material.hpp"

class Shape {
private:
    Material material;

public:
    Shape(Material _material) : material(_material) {}

    Material get_material() const { return material; }

    virtual std::tuple<bool, glm::vec3> ray_hit_position(glm::vec3 ray_origin, glm::vec3 ray_direction) = 0;
    
    virtual glm::vec3 surface_normal(glm::vec3 surface_point) = 0;
};

// glm::vec3 Shape::render(float light_intensity, glm::vec3 incoming_light, glm::vec3 surface_normal)
// {
//     const float diffuse = light_intensity * glm::dot(incoming_light, surface_normal) / glm::pi<float>();
//     return material.get_color() * diffuse;
// }