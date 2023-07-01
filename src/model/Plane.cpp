#include "model/Plane.hpp"

std::tuple<bool, glm::vec3> Plane::ray_hit_position(glm::vec3 ray_origin, glm::vec3 ray_direction)
{
    float cos_a = glm::dot(ray_direction, normal);
    float epsilon = std::numeric_limits<float>().epsilon();

    if (-epsilon < cos_a && cos_a < epsilon) {
        return {false, glm::vec3()};
    }

    glm::vec3 ray_delta = this->get_position() - ray_origin;

    float t = glm::dot(ray_delta, normal) / cos_a;

    if (t < ((-2) * epsilon)) {
        return {false, glm::vec3()};
    }

    return {true, ray_origin + (ray_direction * t)};
}
