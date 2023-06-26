#include "Image.hpp"

Image::Image(std::filesystem::path file_path)
{
    auto ps = file_path.string();
    auto path = ps.c_str();
    int image_w, image_h, channels;

    stbi_set_flip_vertically_on_load(true);
    auto load = stbi_load(path, &image_w, &image_h, &channels, 3);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, load);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(load);
}

void Image::bind_to_program(GLuint shader_program)
{
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);
}
