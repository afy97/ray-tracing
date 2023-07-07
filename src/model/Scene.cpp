#include "model/Scene.hpp"

void Scene::add_object(Shape* shape)
{
    scene_objects.push_back(shape);
}

std::tuple<bool, glm::vec3, Shape*> Scene::get_surface_intersection(Ray ray, Shape* own) const
{
    std::tuple<bool, glm::vec3, Shape*> result;

    float last = std::numeric_limits<float>().max();

    for (const auto& obj : scene_objects) {
        if (own != obj) {
            bool hit;
            auto point{obj->ray_hit_position(ray, hit)};

            if (hit) {
                const auto temp{point - ray.get_origin()};
                float next = glm::dot(temp, temp);

                if (next < last) {
                    last = next;
                    result = {hit, point, obj};
                }
            }
        }
    }

    return result;
}

glm::vec3 Scene::bounce_light(glm::vec3 point, Shape* object, int remaining_bounces) const
{
    constexpr float pi = glm::pi<float>();
    constexpr float pi_rsp = 1.0f / pi;
    constexpr float p_w_rsp = 2.0f * pi;

    if (0 < (remaining_bounces + 1)) {
        glm::vec3 n = object->surface_normal(point);
        glm::vec3 w = glm::normalize(glm::vec3(
            Random::uniform_float(-1.0f, 1.0f), Random::uniform_float(-1.0f, 1.0f), Random::uniform_float(-1.0f, 1.0f)
        ));

        float cos_a = glm::dot(w, n);

        if (cos_a < 0) {
            w = -w;
            cos_a = -cos_a;
        }

        const auto&& [bounce, next_point, next_object] = get_surface_intersection(Ray(point, w), object);

        if (bounce) {
            glm::vec3 emission{next_object->get_material().get_emission()};
            glm::vec3 color{object->get_material().get_color()};
            glm::vec3 diffuse{(color * pi_rsp) * (cos_a * p_w_rsp)};

            if (0 < remaining_bounces) {
                emission = bounce_light(next_point, next_object, remaining_bounces - 1);
            }

            return glm::vec3(emission.r * diffuse.r, emission.g * diffuse.g, emission.b * diffuse.b) +
                   object->get_material().get_emission();
        }
    }

    return glm::vec3(object->get_material().get_emission());
}
