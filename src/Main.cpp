#define STB_IMAGE_IMPLEMENTATION

#include "pch.hpp"

#include "App.hpp"
#include "Shader.hpp"
#include "Canvas.hpp"
#include "Image.hpp"
#include "Texture.hpp"

#include <random>

static constexpr size_t count = 1280 * 720 * 4;

int main(int argc, char const *argv[])
{
    std::filesystem::path vrt("D:/Documents/Visual Studio 2022/C++/RayTracing/src/shader/shader.vert");
    std::filesystem::path frg("D:/Documents/Visual Studio 2022/C++/RayTracing/src/shader/shader.frag");
    std::filesystem::path img("C:/Users/Afy/Downloads/512x512.png");

    try
    {
        App app(1280, 720);
        Shader shader(vrt, frg);
        Canvas canvas;
        Image image(img);
        Texture texture(1280, 720);

        auto array = new Texture::Color[count];

        // image.bind_to_program(shader.get_program());

        app.add_command([&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            
            for (size_t i = 0; i < count; i++)
            {
                array[i] = Texture::Color{
                    .red = static_cast<uint8_t>(rand() % 255),
                    .green = static_cast<uint8_t>(rand() % 255),
                    .blue = static_cast<uint8_t>(rand() % 255),
                    .alpha = 255,
                };
            }

            texture.update_buffer(array, count);
        });

        app.add_command([&]() {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.get_texture());
            glUseProgram(shader.get_program());
            glBindVertexArray(canvas.get_vao());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        });

        app.run();
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    return EXIT_SUCCESS;
}
