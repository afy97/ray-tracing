#include "Texture.hpp"

Texture::Texture(int _width, int _height) : width(_width), height(_height), count(width * height * 4), buffer(count)
{
    glGenTextures(1, &texture);
}

void Texture::update_buffer(std::vector<glm::u8vec4> update)
{
    constexpr size_t type_size = sizeof(glm::u8vec4);
    
    size_t src_size = update.size();
    size_t dst_size = buffer.size();

    if (dst_size < src_size) {
        throw std::runtime_error("Buffer overflow");
    } else {
        std::memcpy(buffer.data(), update.data(), src_size * type_size);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::bind_to_program(GLuint shader_program)
{
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);
}
