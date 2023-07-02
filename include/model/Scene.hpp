#pragma once

#include "pch.hpp"

#include "Random.hpp"
#include "Ray.hpp"
#include "Shape.hpp"

class Scene {
private:
    std::vector<std::shared_ptr<Shape>> scene_objects;

public:
    void add_object(std::shared_ptr<Shape> shape);

    std::tuple<bool, glm::vec3, std::shared_ptr<Shape>>
    get_surface_intersection(Ray ray, std::shared_ptr<Shape> own = nullptr);

    glm::vec3 bounce_light(glm::vec3 point, std::shared_ptr<Shape> object, int remaining_bounces = 0);
};
