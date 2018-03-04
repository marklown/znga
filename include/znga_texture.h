#ifndef ZNGA_TEXTURE_H
#define ZNGA_TEXTURE_H

#include <GLFW/glfw3.h>
#include <string>

struct znga_texture_t
{
    GLuint texture_id;
};

znga_texture_t znga_create_texture(const std::string& path);

#endif