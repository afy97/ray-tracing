#include "Texture.hpp"

Texture::Texture(int _width, int _height) : width(_width), height(_height), count(width * height * 4)
{
    buffer = new Color[width * height * 4];
    glGenTextures(1, &texture);
}

Texture::~Texture() {
    delete[] buffer;
}

void Texture::update_buffer(const Texture::Color* update, int item_count)
{
    if (count < item_count) {
        throw std::runtime_error("Buffer overflow");
    }

    memcpy_s(buffer, count * sizeof(Texture::Color), update, item_count * sizeof(Texture::Color));

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind_to_program(GLuint shader_program)
{
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);
}
