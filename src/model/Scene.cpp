#include "model/Scene.hpp"

void Scene::add_object(std::shared_ptr<Shape> shape)
{
    scene_objects.push_back(shape);
}

std::tuple<bool, glm::vec3, std::shared_ptr<Shape>> Scene::get_surface_intersection(Ray ray, std::shared_ptr<Shape> own)
{
    auto result = std::make_tuple(false, glm::vec3(), std::shared_ptr<Shape>(nullptr));

    float last = std::numeric_limits<float>().max();

    for (auto&& obj : scene_objects) {
        if (own != obj) {
            auto [hit, point] = obj->ray_hit_position(ray);

            if (hit) {
                float next = glm::length(point - ray.get_origin());

                if (next < last) {
                    last = next;
                    result = {hit, point, obj};
                }
            }
        }
    }

    return result;
}

glm::vec3 Scene::bounce_light(glm::vec3 point, std::shared_ptr<Shape> object, int remaining_bounces)
{
    static constexpr float pi = glm::pi<float>();
    static constexpr float pi_rsp = 1.0f / pi;
    static constexpr float p_w_rsp = 2.0f * pi;

    if (remaining_bounces < 0) {
        return glm::vec3(object->get_material().get_emission());
    }

    auto result = std::make_tuple(glm::vec3(), glm::vec3(), std::shared_ptr<Shape>(nullptr));

    glm::vec3 n = object->surface_normal(point);
    glm::vec3 w = glm::normalize(glm::vec3(
        Random::uniform_float(-1.0f, 1.0f),
        Random::uniform_float(-1.0f, 1.0f),
        Random::uniform_float(-1.0f, 1.0f)
    ));

    float cos_a = glm::dot(w, n);

    if (cos_a < 0) {
        w = -w;
        cos_a = -cos_a;
    }

    auto [bounce, next_point, next_object] = get_surface_intersection(Ray(point, w), object);

    if (bounce) {
        glm::vec3 emission = glm::vec3(next_object->get_material().get_emission());
        glm::vec3 color = object->get_material().get_color();
        glm::vec3 diffuse = (color * pi_rsp) * (cos_a * p_w_rsp);

        if (0 < remaining_bounces) {
            emission = bounce_light(next_point, next_object, remaining_bounces - 1);
        }

        return glm::vec3(emission.r * diffuse.r, emission.g * diffuse.g, emission.b * diffuse.b) + object->get_material().get_emission();
    }

    return glm::vec3(object->get_material().get_emission());
}
