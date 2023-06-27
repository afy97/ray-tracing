#pragma once

#include "pch.hpp"

class Canvas {
private:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

public:
    Canvas();
    ~Canvas();

    GLuint get_vbo() const { return VBO; }
    GLuint get_vao() const { return VAO; }
    GLuint get_ebo() const { return EBO; }
};
