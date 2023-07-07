#pragma once

#include "pch.hpp"

#include "Random.hpp"
#include "Ray.hpp"
#include "Shape.hpp"

class Scene {
private:
    std::vector<Shape*> scene_objects;

public:
    void add_object(Shape* shape);

    std::tuple<bool, glm::vec3, Shape*>
    get_surface_intersection(Ray ray, Shape* own = nullptr) const;

    glm::vec3 bounce_light(glm::vec3 point, Shape* object, int remaining_bounces = 0) const;
};
