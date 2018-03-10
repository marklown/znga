#ifndef ZNGA_TEXTURE_H
#define ZNGA_TEXTURE_H

#include <GLFW/glfw3.h>

#define DIFFUSE_MAP 0
#define SPECULAR_MAP 1
#define NORMAL_MAP 2

typedef struct znga_texture_t
{
    GLuint id;
    uint8_t type;
} znga_texture_t;

znga_texture_t znga_create_texture(const char* path, uint8_t type);

#endif