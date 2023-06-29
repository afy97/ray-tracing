#define STB_IMAGE_IMPLEMENTATION

#include "pch.hpp"

#include "App.hpp"
#include "Canvas.hpp"
#include "Image.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

static constexpr int width = 1280;
static constexpr int height = 720;
static constexpr int count = width * height;

int main(int argc, char const* argv[])
{
    std::filesystem::path vrt("C:/Users/Afy/Documents/Projects/C++/ray-tracing/src/shader/shader.vert");
    std::filesystem::path frg("C:/Users/Afy/Documents/Projects/C++/ray-tracing/src/shader/shader.frag");

    auto buffer = new std::vector<Texture::Color>(count);

    try {
        App app(width, height);
        Shader shader(vrt, frg);
        Canvas canvas;
        Texture texture(width, height);

        float fov = glm::radians(30.0f);
        float per_pixel_angle = fov / height;
        float radius = 2.0f;
        float distance = 10.0f;

        glm::vec3 forward(0.0f, 0.0f, -1.0f);
        glm::vec3 right(1.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 position = forward * distance;
        glm::vec3 light(4.0f, 8.0f, -2.0f);
        glm::vec3 light_direction = glm::normalize(light - position);
        glm::vec3 rgb = glm::normalize(glm::vec3(0.33f, 0.33f, 1.0f)) * 255.0f;

        float light_distance = glm::length(light - position);

        auto result = std::async([&]() {
            std::for_each(std::execution::par, buffer->begin(), buffer->end(), [&](Texture::Color& c) {
                int index = (&c) - buffer->data();

                float x = (index % width) - (width / 2);
                float y = (index / width) - (height / 2);

                glm::vec3 v_y = glm::rotate(forward, y * per_pixel_angle, right);
                glm::vec3 v_x = glm::rotate(v_y, (-x) * per_pixel_angle, up);

                float t_ca = glm::dot(position, v_x);
                float d_sq = std::pow(distance, 2) - std::pow(t_ca, 2);

                if (0 < d_sq) {
                    float t_hc = std::sqrt(std::pow(radius, 2) - d_sq);
                    float d_v = t_ca - t_hc;

                    glm::vec3 v = v_x * d_v;

                    if (glm::length(v - light) < light_distance) {
                        glm::vec3 n = glm::normalize(v - position);
                        glm::vec3 color = rgb * std::powf(glm::dot(light_direction, n), 2);

                        c = {
                            .red = static_cast<uint8_t>(color.r),
                            .green = static_cast<uint8_t>(color.g),
                            .blue = static_cast<uint8_t>(color.b),
                            .alpha = 255,
                        };
                    }
                }
            });
        });

        app.add_command([&]() {
            texture.update_buffer(buffer->data(), count);
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

    delete buffer;

    return EXIT_SUCCESS;
}
