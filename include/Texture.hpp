#pragma once

#include "pch.hpp"

class Texture
{
public:
    struct Color
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    };

private:
    const int width;
    const int height;
    const int count;
    Color *buffer;
    GLuint texture;

public:
    Texture(int width, int height);
    ~Texture();

    void bind_to_program(GLuint shader_program);
    void update_buffer(const Color *update, int item_count);

    GLuint get_texture() const { return texture; }
};
