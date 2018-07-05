
#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>

namespace Znga {
namespace Graphics {

Shader LoadShader(const std::string& vPath, const std::string& fPath)
{
    GLuint program, vertex_shader, fragment_shader;
    GLint success;
    char info_log[512];

    const char* vertex_shader_src = ReadFile(vPath);
    const char* fragment_shader_src = ReadFile(fPath);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", info_log);
    }

    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", info_log);
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    free((char*)vertex_shader_src);
    free((char*)fragment_shader_src);

    return program;
}

void DeleteShader(Shader& shader)
{
    glDeleteProgram(shader);
    shader = 0;
}

void UseShader(Shader shader)
{
    glUseProgram(shader);
}

Uniform GetUniformLoc(Shader shader, const std::string& uniform)
{
    return glGetUniformLocation(shader, uniform.c_str());
}

void SetUniformMat4(Uniform location, const GLfloat* values)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

void SetUniformVec3(Uniform location, const GLfloat* values)
{
    glUniform3fv(location, 1, values);
}

void SetUniform1f(Uniform location, const GLfloat value)
{
    glUniform1f(location, value);
}

const char* ReadFile(const std::string& path)
{
    char* buffer = nullptr;
    size_t size = 0;

    FILE* fp = fopen(path.c_str(), "r");
    if (!fp) {
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    buffer = (char*)malloc((size + 1) * sizeof(*buffer));
    fread(buffer, size, 1, fp);
    buffer[size] = '\0';

    return buffer;
}

} // namespace Graphics
} // namespace Znga