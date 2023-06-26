#pragma once

#include "pch.hpp"

class Image
{
private:
    GLuint texture;

public:
    Image(std::filesystem::path file_path);

    void bind_to_program(GLuint shader_program);

    GLuint get_texture() const { return texture; }
};
