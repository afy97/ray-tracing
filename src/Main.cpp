#define STB_IMAGE_IMPLEMENTATION

#include "pch.hpp"

#include "App.hpp"
#include "Canvas.hpp"
#include "Image.hpp"
#include "Random.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "model/Plane.hpp"
#include "model/Scene.hpp"
#include "model/Sphere.hpp"

static constexpr int width = 4 * 128;
static constexpr int height = 3 * 128;
static constexpr int count = width * height;
static constexpr int samples = 128;
static constexpr int light_bounces = 3;

int main(int argc, char const* argv[])
{
    try {
        std::filesystem::path vrt("shaders/shader.vert");
        std::filesystem::path frg("shaders/shader.frag");

        App app(width, height);
        Shader shader(vrt, frg);
        Canvas canvas;
        Texture texture(width, height);

        std::vector<Texture::Color> buffer(count);
        std::mutex guard;

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
        Material red_material(glm::vec3(1.0f, 0.33f, 0.33f));
        Material green_material(glm::vec3(0.33f, 1.0f, 0.33f));
        Material blue_material(glm::vec3(0.33f, 0.33f, 1.0f));
        Material emissive_material(glm::vec3(1.0f), 8.0f);

        Scene scene;
        scene.add_object(std::make_shared<Plane>(default_material, -up, up));
        scene.add_object(std::make_shared<Plane>(green_material, -right * 3.0f, right));
        scene.add_object(std::make_shared<Plane>(red_material, right * 3.0f, -right));
        scene.add_object(std::make_shared<Plane>(default_material, forward * 11.0f, -forward));
        scene.add_object(std::make_shared<Sphere>(blue_material, forward * 10.0f, 1.0f));
        scene.add_object(std::make_shared<Sphere>(emissive_material, glm::vec3(1.0f, 3.0f, -8.0f), 1.0f));

        auto result = std::async([&]() {
            std::for_each(std::execution::par, buffer.begin(), buffer.end(), [&](Texture::Color& c) {
                int index = (&c) - buffer.data();

                float x = (index % width) - (width / 2);
                float y = (index / width) - (height / 2);

                glm::vec3 v_y = glm::rotate(forward, y * per_pixel_angle, right);
                glm::vec3 v_x = glm::rotate(v_y, (-x) * per_pixel_angle, up);
                glm::vec3 hit_point = origin;
                glm::vec3 diffuse(0.0f);

                auto [hit, point, object] = scene.get_surface_intersection(Ray(origin, v_x));

                if (hit) {
                    float emission = object->get_material().get_emission();

                    if (0.0f < emission) {
                        glm::vec3 color = object->get_material().get_color();
                        glm::u8vec3 shade = glm::clamp(color * (255.0f * emission), 0.0f, 255.0f);
                        std::lock_guard lock(guard);

                        buffer[index] = {
                            .red = shade.r,
                            .green = shade.g,
                            .blue = shade.b,
                            .alpha = 255,
                        };
                    } else {
                        for (int i = 0; i < samples; i++) {
                            diffuse += scene.bounce_light(point, object, light_bounces);
                        }

                        glm::u8vec3 gain = glm::clamp(diffuse * (255.0f / samples), 0.0f, 255.0f);
                        std::lock_guard lock(guard);

                        buffer[index] = {
                            .red = gain.r,
                            .green = gain.g,
                            .blue = gain.b,
                            .alpha = 255,
                        };
                    }
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
