#ifndef ZNGA_TEXTURE_H
#define ZNGA_TEXTURE_H

#include <GLFW/glfw3.h>
#include <string>
#include <map>

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

struct TextureInfo 
{
    // UV coordinates, order is bottom left, bottom right, top right, top left
    GLfloat u[4];
    GLfloat v[4];
};

class TextureAtlas
{
public:
    TextureAtlas(const std::string& texturePath, const std::string& infoPath);
    virtual ~TextureAtlas();
    TextureInfo GetTextureInfo(const std::string& name);
private:

    std::map<std::string, TextureInfo> m_textureInfo;
    Texture m_texture;
    GLuint m_w;
    GLuint m_h;
};

} // namespace Graphics
} // namespace Znga

#endif