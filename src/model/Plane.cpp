#include "model/Plane.hpp"

std::tuple<bool, glm::vec3> Plane::ray_hit_position(Ray ray)
{
    float cos_a = glm::dot(ray.get_direction(), normal);
    float epsilon = std::numeric_limits<float>().epsilon();

    if (-epsilon < cos_a && cos_a < epsilon) {
        return {false, glm::vec3()};
    }

    glm::vec3 ray_delta = this->get_position() - ray.get_origin();

    float t = glm::dot(ray_delta, normal) / cos_a;

    if (t < ((-2) * epsilon)) {
        return {false, glm::vec3()};
    }

    return {true, ray.get_origin() + (ray.get_direction() * t)};
}
