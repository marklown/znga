#ifndef ZNGA_SHADER_H
#define ZNGA_SHADER_H

#include <GLFW/glfw3.h>

typedef GLuint znga_shader_t;

const char* znga_read_file(const char* path);
znga_shader_t znga_create_shader(const char* vpath, const char* fpath);

#endif