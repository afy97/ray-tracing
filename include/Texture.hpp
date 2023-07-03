#pragma once

#include "pch.hpp"

class Texture {
private:
    const int width;
    const int height;
    const int count;
    
    std::vector<glm::u8vec4> buffer;
    
    GLuint texture;

public:
    Texture(int width, int height);

    void bind_to_program(GLuint shader_program);
    void update_buffer(std::vector<glm::u8vec4> update);

    GLuint get_texture() const { return texture; }
};
