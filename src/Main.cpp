#define STB_IMAGE_IMPLEMENTATION

#include "pch.hpp"

#include "App.hpp"
#include "Canvas.hpp"
#include "Image.hpp"
#include "Random.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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

        glm::vec3 forward(0.0f, 0.0f, -1.0f);
        glm::vec3 right(1.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        Material default_material;
        Material light_material(glm::vec3(1.0f), 1.0f);

        Sphere ball(forward * 10.0f, 2.0f, default_material);
        Sphere light(glm::vec3(2.0, 4.0, -4.0f), 2.0f, light_material);

        auto result = std::async([&]() {
            std::for_each(std::execution::par, buffer.begin(), buffer.end(), [&](Texture::Color& c) {
                int index = (&c) - buffer.data();

                float x = (index % width) - (width / 2);
                float y = (index / width) - (height / 2);

                glm::vec3 v_y = glm::rotate(forward, y * per_pixel_angle, right);
                glm::vec3 v_x = glm::rotate(v_y, (-x) * per_pixel_angle, up);

                auto [valid, v] = ball.ray_hit_position(glm::vec3(0.0f), v_x);

                if (!valid) {
                    return;
                }

                glm::vec3 n = ball.surface_normal(v);
                glm::vec3 acc(0.0f);

                for (size_t i = 0; i < samples; i++) {
                    float x1 = Random::uniform_float(0.0f, 1.0f);
                    float x2 = Random::uniform_float(0.0f, 1.0f);
                    float x3 = Random::uniform_float(0.0f, 1.0f);

                    glm::vec3 w = glm::normalize(glm::vec3(x1, x2, x3));

                    float cos_a = glm::dot(w, n);

                    if (cos_a < 0) {
                        w = -w;
                        cos_a = -cos_a;
                    }

                    auto [hit, _] = light.ray_hit_position(v, w);

                    if (hit) {
                        acc += (pi_rsp * p_w_rsp * cos_a) * light.get_material().get_emmission();
                    }
                }

                glm::vec3 gain = acc * (255.0f / samples);
                std::lock_guard lock(guard);

                buffer[index] = {
                    .red = static_cast<uint8_t>(gain.r),
                    .green = static_cast<uint8_t>(gain.g),
                    .blue = static_cast<uint8_t>(gain.b),
                    .alpha = 255,
                };
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
