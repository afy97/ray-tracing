#include "model/Sphere.hpp"

std::tuple<bool, glm::vec3> Sphere::ray_hit_position(glm::vec3 ray_origin, glm::vec3 ray_direction)
{
    float t_ca, d_sq, r_sq, t_hc, d_v;

    glm::vec3 ray_delta = this->get_position() - ray_origin;

    t_ca = glm::dot(ray_delta, ray_direction);
    
    if (t_ca < 0) {
        return {false, glm::vec3()};
    }
    
    d_sq = std::pow(glm::length(ray_delta), 2) - std::pow(t_ca, 2);

    if (d_sq < 0) {
        return {false, glm::vec3()};
    }

    r_sq = std::pow(radius, 2);

    if (r_sq < d_sq) {
        return {false, glm::vec3()};
    }

    t_hc = std::sqrt(r_sq - d_sq);
    d_v = t_ca - t_hc;

    return {true, ray_origin + (ray_direction * d_v)};
}

glm::vec3 Sphere::surface_normal(glm::vec3 surface_point) {
    return glm::normalize(surface_point - this->get_position());
}
