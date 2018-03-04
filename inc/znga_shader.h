#ifndef ZNGA_SHADER_H
#define ZNGA_SHADER_H

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>

typedef GLuint znga_shader_t;

std::string znga_read_file(const std::string& path)
{
    std::string contents;
    std::ifstream stream(path, std::ios::in);
    if (!stream.is_open()) {
        printf("Could not read file %s\n", path.c_str());
        return "";
    }

    std::string line = "";
    while (!stream.eof()) {
        std::getline(stream, line);
        contents.append(line + "\n");
    }

    stream.close();
    return contents;
}

znga_shader_t znga_create_shader(const std::string& vpath, const std::string& fpath)
{
    znga_shader_t program, vertex_shader, fragment_shader;
    GLint success;
    char info_log[512];

    std::string vertex_shader_str = znga_read_file(vpath);
    std::string fragment_shader_str = znga_read_file(fpath);
    const char* vertex_shader_src = vertex_shader_str.c_str();
    const char* fragment_shader_src = fragment_shader_str.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

#endif