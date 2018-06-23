
#include "znga_shader.h"
#include <stdio.h>
#include <stdlib.h>

namespace Znga {
namespace Graphics {

Shader::Shader(const std::string& vPath, const std::string& fPath)
{
    GLuint program, vertex_shader, fragment_shader;
    GLint success;
    char info_log[512];

    const char* vertex_shader_src = ReadFile(vPath);
    const char* fragment_shader_src = ReadFile(fPath);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", info_log);
    }

    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
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

    m_id = program;
    m_uniformModel = GetUniformLocation("u_model");
    m_uniformView = GetUniformLocation("u_view");
    m_uniformProjection = GetUniformLocation("u_projection");
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

const char* Shader::ReadFile(const std::string& path) const
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

GLuint Shader::GetUniformLocation(const std::string& uniform) const
{
    return glGetUniformLocation(m_id, uniform.c_str());
}

void Shader::Enable() const
{
    glUseProgram(m_id);
}

void Shader::Disable() const
{
    glUseProgram(0);
}

void Shader::SetUniformMat4(GLuint location, const GLfloat* values) const
{
    glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

void Shader::SetUniformVec3(GLuint location, const GLfloat* values) const
{
    glUniform3fv(location, 1, values);
}

} // namespace Graphics
} // namespace Znga