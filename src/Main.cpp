#define STB_IMAGE_IMPLEMENTATION

#include "pch.hpp"

#include "App.hpp"
#include "Canvas.hpp"
#include "Image.hpp"
#include "Random.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "model/Plane.hpp"
#include "model/Sphere.hpp"

static constexpr int width = 1280;
static constexpr int height = 720;
static constexpr int count = width * height;
static constexpr int samples = 64;

int main(int argc, char const* argv[])
{
    std::filesystem::path vrt("C:/Users/Afy/Documents/Projects/C++/ray-tracing/src/shader/shader.vert");
    std::filesystem::path frg("C:/Users/Afy/Documents/Projects/C++/ray-tracing/src/shader/shader.frag");

    std::vector<Texture::Color> buffer(count);
    std::mutex guard;

    try {
        App app(width, height);
        Shader shader(vrt, frg);
        Canvas canvas;
        Texture texture(width, height);

        float fov = glm::radians(30.0f);
        float per_pixel_angle = fov / height;
        float pi = glm::pi<float>();
        float pi_rsp = 1.0f / pi;
        float p_w_rsp = 2.0f * pi;
        float p_w = 1.0f / p_w_rsp;
        float f_max = std::numeric_limits<float>().max();

        glm::vec3 forward(0.0f, 0.0f, -1.0f);
        glm::vec3 right(1.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 origin(0.0f);

        Material default_material;
        Material light_material(glm::vec3(1.0f), 10.0f);

        Plane ground(default_material, -up, up);
        Plane left_wall(default_material, -right * 4.0f, right);
        Plane right_wall(default_material, right * 4.0f, -right);
        Plane back_wall(default_material, forward * 12.0f, -forward);
        Sphere ball(default_material, forward * 10.0f, 1.0f);
        Sphere light(light_material, glm::vec3(3.0f, 4.0f, -5.0f), 1.0f);

        std::vector<Shape*> scene;
        scene.push_back(&ground);
        scene.push_back(&left_wall);
        scene.push_back(&back_wall);
        scene.push_back(&right_wall);
        scene.push_back(&ball);
        scene.push_back(&light);

        auto result = std::async([&]() {
            std::for_each(std::execution::par, buffer.begin(), buffer.end(), [&](Texture::Color& c) {
                Shape* debug = &back_wall;

                int index = (&c) - buffer.data();

                float x = (index % width) - (width / 2);
                float y = (index / width) - (height / 2);
                float last = f_max;

                Shape* current = nullptr;

                glm::vec3 acc(0.0f);
                glm::vec3 hit_point = origin;
                glm::vec3 v_y = glm::rotate(forward, y * per_pixel_angle, right);
                glm::vec3 v_x = glm::rotate(v_y, (-x) * per_pixel_angle, up);

                for (auto&& shape : scene) {
                    auto [intersected, intersection] = shape->ray_hit_position(origin, v_x);

                    if (intersected) {
                        float next = glm::length(intersection - origin);

                        if (next < last) {
                            last = next;
                            current = shape;
                            hit_point = intersection;
                        }
                    }
                }

                if (current != nullptr) {
                    glm::vec3 n = current->surface_normal(hit_point);

                    for (size_t i = 0; i < samples; i++) {
                        glm::vec3 w = glm::normalize(glm::vec3(
                            Random::uniform_float(-1.0f, 1.0f),
                            Random::uniform_float(-1.0f, 1.0f),
                            Random::uniform_float(-1.0f, 1.0f)
                        ));

                        Shape* closest = nullptr;

                        float cos_a = glm::dot(w, n);
                        last = f_max;

                        if (cos_a < 0) {
                            w = -w;
                            cos_a = -cos_a;
                        }

                        for (auto&& other : scene) {
                            if (current != other) {
                                auto [intersected, intersection] = other->ray_hit_position(hit_point, w);

                                if (intersected) {
                                    float next = glm::length(intersection - hit_point);

                                    if (next < last) {
                                        closest = other;
                                        last = next;
                                    }
                                }
                            }
                        }

                        if (closest != nullptr) {
                            acc += (pi_rsp * p_w_rsp * cos_a) * closest->get_material().get_emmission();
                        }
                    }

                    glm::vec3 gain = glm::clamp(acc * (255.0f / samples), 0.0f, 255.0f);
                    std::lock_guard lock(guard);

                    buffer[index] = {
                        .red = static_cast<uint8_t>(gain.r),
                        .green = static_cast<uint8_t>(gain.g),
                        .blue = static_cast<uint8_t>(gain.b),
                        .alpha = 255,
                    };
                }
            });
        });

        app.add_command([&]() {
            std::lock_guard lock(guard);
            texture.update_buffer(buffer.data(), count);
        });

        app.add_command([&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.get_texture());
            glUseProgram(shader.get_program());
            glBindVertexArray(canvas.get_vao());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        });

        app.run();
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    return EXIT_SUCCESS;
}
