#ifndef ZNGA_TEXTURE_H
#define ZNGA_TEXTURE_H

#include <GLFW/glfw3.h>
#include <string>

namespace Znga {
namespace Graphics {

const uint8_t DIFFUSE_MAP = 0;
const uint8_t SPECULAR_MAP = 1;
const uint8_t NORMAL_MAP = 2;

struct Texture
{
    GLuint id = 0;
    uint8_t type = DIFFUSE_MAP;
};

Texture LoadTexture(const std::string& path, uint8_t type);

} // namespace Graphics
} // namespace Znga

#endif