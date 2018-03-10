#include "znga_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <OpenGL/GL3.h>

znga_texture_t znga_create_texture(const char* path, uint8_t type)
{
    znga_texture_t texture;
    texture.type = type;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, num_channels;
    unsigned char* data = stbi_load(path, &width, &height, &num_channels, 0);
    if (!data)
    {
        printf("Failed to read texture file: %s\n", path);
        return texture;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}