#include "Shader.hpp"

Shader::Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path)
{
    std::ifstream vert(vertex_path);
    std::ifstream frag(fragment_path);
    std::stringstream v_buf;
    std::stringstream f_buf;
    GLuint vert_shader;
    GLuint frag_shader;
    GLint success;

    v_buf << vert.rdbuf();
    f_buf << frag.rdbuf();

    vert.close();
    frag.close();

    auto v_code = v_buf.str();
    auto f_code = f_buf.str();
    auto v_code_c = v_code.c_str();
    auto f_code_c = f_code.c_str();

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &v_code_c, NULL);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(vert_shader, 1024, NULL, log);
        throw std::runtime_error(log);
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &f_code_c, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(frag_shader, 1024, NULL, log);
        throw std::runtime_error(log);
    }

    program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(frag_shader, 1024, NULL, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}
