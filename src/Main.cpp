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

    auto buffer = new Texture::Color[height][width];

    try {
        App app(width, height);
        Shader shader(vrt, frg);
        Canvas canvas;
        Texture texture(width, height);

        auto result = std::async([&]() {
            auto fov = glm::radians(30.0f);
            auto per_pixel_angle = fov / height;

            glm::vec3 forward(0.0f, 0.0f, -1.0f);
            glm::vec3 right(1.0f, 0.0f, 0.0f);
            glm::vec3 up(0.0f, 1.0f, 0.0f);

            for (int i = 0; i < height; i++) {
                float y = i - (height / 2);
                glm::vec3 v1 = glm::rotate(forward, y * per_pixel_angle, right);

                for (int j = 0; j < width; j++) {
                    float x = j - (width / 2);
                    glm::vec3 v2 = glm::rotate(v1, (-x) * per_pixel_angle, up);
                    glm::vec3 v = v2;
                    v *= 127.0f;
                    v += 127.0f;

                    buffer[i][j] = {
                        .red = static_cast<uint8_t>(v.x),
                        .green = static_cast<uint8_t>(v.y),
                        .blue = static_cast<uint8_t>(v.z),
                        .alpha = 255,
                    };
                }
            }
        });

        app.add_command([&]() {
            static bool skip_flag = false;
            if (!skip_flag && result.wait_for(std::chrono::milliseconds(100)) == std::future_status::ready) {
                texture.update_buffer(&(buffer[0][0]), count);
                skip_flag = true;
            }
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

    delete[] buffer;

    return EXIT_SUCCESS;
}
