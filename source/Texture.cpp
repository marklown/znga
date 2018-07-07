#include "Texture.h"
#include "picojson.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <OpenGL/GL3.h>
#include <map>
#include <vector>

namespace Znga {
namespace Graphics {

Texture LoadTexture(const std::string& path, uint8_t type)
{
    Texture texture;

    texture.type = type;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);
    if (!data) {
        printf("Failed to read texture file: %s\n", path.c_str());
        return Texture();
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return texture;
}


TextureAtlas::TextureAtlas(const std::string& path, const std::string& infoPath)
{
    m_texture = LoadTexture(path, DIFFUSE_MAP);

    char* buffer = nullptr;
    size_t size = 0;
    FILE* fp = fopen(infoPath.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    buffer = (char*)malloc((size + 1) * sizeof(*buffer));
    fread(buffer, size, 1, fp);
    buffer[size] = '\0';
    std::string json_string(buffer);
    free(buffer);

    picojson::value va;
    std::string err = picojson::parse(va, json_string);
    if (!err.empty()) {
        std::cerr << err << std::endl;
    } else {
        if (va.is<picojson::object>()) {
            const picojson::object& ob = va.get<picojson::object>();
            auto it = ob.find("meta");
            if (it != ob.end()) {
                const picojson::value& v = it->second;
                if (v.is<picojson::object>()) {
                    const picojson::object& o = v.get<picojson::object>();
                    auto it = o.find("size");
                    if (it != o.end()) {
                        const picojson::value& v = it->second;
                        if (v.is<picojson::object>()) {
                            const picojson::object& o = v.get<picojson::object>();
                            auto it = o.find("w");
                            if (it != o.end()) {
                                const picojson::value& v = it->second;
                                if (v.is<double>()) {
                                    m_w = (GLuint)v.get<double>();
                                }
                            }
                            it = o.find("h");
                            if (it != o.end()) {
                                const picojson::value& v = it->second;
                                if (v.is<double>()) {
                                    m_h = (GLuint)v.get<double>();
                                }
                            }
                        }
                    }
                }
            }
            auto it2 = ob.find("frames");
            if (it2 != ob.end()) {
                const picojson::value& v = it2->second;
                if (v.is<picojson::array>()) {
                    const picojson::array& a = v.get<picojson::array>();
                    for (auto v : a) {
                        TextureInfo info;
                        std::string frame_name;
                        GLfloat x, y, w, h;
                        if (v.is<picojson::object>()) {
                            const picojson::object& o = v.get<picojson::object>();
                            auto it = o.find("filename");
                            if (it != o.end()) {
                                const picojson::value& v = it->second;
                                if (v.is<std::string>()) {
                                    frame_name = v.get<std::string>();
                                }
                            }
                            it = o.find("frame");
                            if (it != o.end()) {
                                const picojson::value& v = it->second;
                                if (v.is<picojson::object>()) {
                                    const picojson::object& o = v.get<picojson::object>();
                                    auto it = o.find("x");
                                    if (it != o.end()) {
                                        const picojson::value& v = it->second;
                                        if (v.is<double>()) {
                                            x = (GLfloat)v.get<double>();
                                        }
                                    }
                                    it = o.find("y");
                                    if (it != o.end()) {
                                        const picojson::value& v = it->second;
                                        if (v.is<double>()) {
                                            y = (GLfloat)v.get<double>();
                                        }
                                    }
                                    it = o.find("w");
                                    if (it != o.end()) {
                                        const picojson::value& v = it->second;
                                        if (v.is<double>()) {
                                            w = (GLfloat)v.get<double>();
                                        }
                                    }
                                    it = o.find("h");
                                    if (it != o.end()) {
                                        const picojson::value& v = it->second;
                                        if (v.is<double>()) {
                                            h = (GLfloat)v.get<double>();
                                        }
                                    }
                                    info.u[0] = x / m_w;
                                    info.v[0] = y / m_h;
                                    info.u[1] = (x+w) / m_w;
                                    info.v[1] = y / m_h;
                                    info.u[2] = (x+w) / m_w;
                                    info.v[2] = (y+h) / m_h;
                                    info.u[3] = x / m_w;
                                    info.v[3] = (y+h) / m_h;
                                    m_textureInfo[frame_name] = info;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

TextureAtlas::~TextureAtlas()
{
    // todo delete the underlying texture and invalidate the map
}

TextureInfo TextureAtlas::GetTextureInfo(const std::string& name)
{
    auto it = m_textureInfo.find(name);
    if (it != m_textureInfo.end()) {
        return it->second;
    }
    return TextureInfo();
}


} // namespace Graphics
} // namespace Znga