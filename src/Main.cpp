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

constexpr float byte_min = static_cast<float>(std::numeric_limits<uint8_t>().min());
constexpr float byte_max = static_cast<float>(std::numeric_limits<uint8_t>().max());

int main(int argc, char const* argv[])
{
    try {
        int width = 4 * 128;
        int height = 3 * 128;
        int samples = 128;
        int bounces = 3;

        if (1 < argc) {
            if (argc % 2 != 0) {
                for (int i = 1; i < argc; i += 2) {
                    std::string option(*(argv + i));

                    int value = std::stoi(std::string(*(argv + (i + 1))));

                    if (option == "--width") {
                        width = std::max(width, value);
                    } else if (option == "--height") {
                        height = std::max(height, value);
                    } else if (option == "--samples") {
                        samples = std::max(value, 0);
                    } else if (option == "--bounces") {
                        bounces = std::max(value, 0);
                    } else {
                        throw std::runtime_error("Invalid argument " + option);
                    }
                }
            } else {
                throw std::runtime_error("Invalid arguments");
            }
        }

        int count = width * height;

        std::filesystem::path vrt("shaders/shader.vert");
        std::filesystem::path frg("shaders/shader.frag");

        App app(width, height);
        Shader shader(vrt, frg);
        Canvas canvas;
        Texture texture(width, height);

        std::vector<glm::u8vec4> buffer(count);
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

        Plane ground(default_material, -up, up);
        Plane left_wall(green_material, -right * 3.0f, right);
        Plane right_wall(red_material, right * 3.0f, -right);
        Plane back_wall(default_material, forward * 11.0f, -forward);
        Sphere ball(blue_material, forward * 10.0f, 1.0f);
        Sphere light(emissive_material, glm::vec3(1.0f, 3.0f, -8.0f), 1.0f);

        Scene scene;
        scene.add_object(&ground);
        scene.add_object(&left_wall);
        scene.add_object(&right_wall);
        scene.add_object(&back_wall);
        scene.add_object(&ball);
        scene.add_object(&light);

        int thread_count = std::thread::hardware_concurrency() - 1;
        int pixels_per_thread = count / thread_count;
        int outlier_pixels = count % thread_count;

        std::vector<std::thread> thread_pool;
        thread_pool.reserve(thread_count);

        for (int i = 0; i <= thread_count; i++) {
            int start_index = i * pixels_per_thread;
            int end_index = start_index + pixels_per_thread;

            if (i == thread_count) {
                end_index = outlier_pixels;
            }

            thread_pool.push_back(std::thread(
                [&](int start, int end) {
                    for (int index = start; index < end; index++) {
                        float x = (index % width) - (width / 2);
                        float y = (index / width) - (height / 2);

                        glm::vec3 v_y = glm::rotate(forward, y * per_pixel_angle, right);
                        glm::vec3 v_x = glm::rotate(v_y, (-x) * per_pixel_angle, up);
                        glm::vec3 hit_point{origin};
                        glm::vec3 diffuse{0.0f};

                        auto [hit, point, object] = scene.get_surface_intersection(Ray(origin, v_x));

                        if (hit) {
                            for (int i = 0; i < samples; i++) {
                                diffuse += scene.bounce_light(point, object, bounces);
                            }

                            std::lock_guard lock(guard);
                            buffer[index] =
                                glm::u8vec4{glm::clamp(diffuse * (byte_max / samples), byte_min, byte_max), byte_max};
                        }
                    }
                },
                start_index,
                end_index
            ));
        }

        std::thread tone_mapping = std::thread([&]() {
            for (auto& thread : thread_pool) {
                thread.join();
            }

            // TODO: tone mapping
        });

        app.add_command([&]() {
            std::lock_guard lock(guard);
            texture.update_buffer(buffer);
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

        tone_mapping.join();
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    return EXIT_SUCCESS;
}
