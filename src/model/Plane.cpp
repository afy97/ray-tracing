#include "model/Plane.hpp"

glm::vec3 Plane::ray_hit_position(Ray ray, bool& hit)
{
    float cos_a, epsilon, t;

    cos_a = glm::dot(ray.get_direction(), normal);
    epsilon = std::numeric_limits<float>().epsilon();

    if (epsilon < std::abs(cos_a)) {
        t = glm::dot(get_position() - ray.get_origin(), normal) / cos_a;

        if ((-0.0f) < t) {
            hit = true;
            return ray.get_origin() + (ray.get_direction() * t);
        }
    }

    hit = false;

    return glm::vec3();
}
