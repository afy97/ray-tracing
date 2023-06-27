#pragma once

#include "pch.hpp"

class Shader {
private:
    GLuint program;

public:
    Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path);

    GLuint get_program() const { return program; }
};
