#pragma once

#include "pch.hpp"

#include "Ray.hpp"
#include "Shape.hpp"

class Scene {
private:
    std::vector<std::shared_ptr<Shape>> scene_objects;

public:
    void add_object(std::shared_ptr<Shape> shape);

    std::tuple<bool, glm::vec3, std::shared_ptr<Shape>> get_surface_intersection(Ray ray, std::shared_ptr<Shape> own);
};

void Scene::add_object(std::shared_ptr<Shape> shape)
{
    scene_objects.push_back(shape);
}

std::tuple<bool, glm::vec3, std::shared_ptr<Shape>>
Scene::get_surface_intersection(Ray ray, std::shared_ptr<Shape> own = nullptr)
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
