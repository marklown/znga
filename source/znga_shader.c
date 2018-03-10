
#include "znga_shader.h"
#include <stdio.h>
#include <stdlib.h>

const char* znga_read_file(const char* path)
{
    char* buffer = NULL;
    size_t size = 0;

    FILE* fp = fopen(path, "r");
    if (!fp)
    {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    buffer = malloc((size + 1) * sizeof(*buffer));
    fread(buffer, size, 1, fp);
    buffer[size] = '\0';

    return buffer;
}

znga_shader_t znga_create_shader(const char* vpath, const char* fpath)
{
    znga_shader_t program, vertex_shader, fragment_shader;
    GLint success;
    char info_log[512];

    const char* vertex_shader_src = znga_read_file(vpath);
    const char* fragment_shader_src = znga_read_file(fpath);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", info_log);
    }

    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
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

    return program;
}