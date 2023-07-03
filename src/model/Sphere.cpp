#include "model/Sphere.hpp"

glm::vec3 Sphere::ray_hit_position(Ray ray, bool& hit)
{
    float t_ca, d_sq, r_sq, t_hc, d_v;

    glm::vec3 ray_delta{get_position() - ray.get_origin()};

    t_ca = glm::dot(ray_delta, ray.get_direction());

    if ((-0.0f) < t_ca) {
        d_sq = glm::dot(ray_delta, ray_delta) - std::pow(t_ca, 2.0f);

        if ((-0.0f) < d_sq) {
            r_sq = std::pow(radius, 2.0f);

            if (d_sq < r_sq) {
                t_hc = std::sqrt(r_sq - d_sq);
                d_v = t_ca - t_hc;
                hit = true;

                return ray.get_origin() + (ray.get_direction() * d_v);
            }
        }
    }

    hit = false;

    return glm::vec3();
}

glm::vec3 Sphere::surface_normal(glm::vec3 surface_point)
{
    return glm::normalize(surface_point - get_position());
}
