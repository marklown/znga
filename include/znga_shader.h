#ifndef ZNGA_SHADER_H
#define ZNGA_SHADER_H

#include <GLFW/glfw3.h>
#include <string>

namespace Znga {
namespace Graphics {

class Shader
{
public:
    Shader() = default;
    Shader(const std::string& vPath, const std::string& fPath);
    virtual ~Shader();

    GLuint GetUniformLocation(const std::string& uniform) const;
    void SetUniformMat4(GLuint location, const GLfloat* values) const;
    void SetUniformVec3(GLuint location, const GLfloat* values) const;

    void Enable() const;
    void Disable() const;

private:
    const char* ReadFile(const std::string& path) const;

    GLuint m_id;
    GLuint m_uniformModel;
    GLuint m_uniformView;
    GLuint m_uniformProjection;
    GLuint m_uniformLightDir;
    GLuint m_uniformLightColor;
    GLuint m_uniformObjectColor;
};

}
}

#endif