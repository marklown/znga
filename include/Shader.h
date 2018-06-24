#ifndef ZNGA_SHADER_H
#define ZNGA_SHADER_H

#include <GLFW/glfw3.h>
#include <string>

namespace Znga {
namespace Graphics {

using Shader = GLuint;
using Uniform = GLuint;

Shader LoadShader(const std::string& vPath, const std::string& fPath);
void DeleteShader(Shader& shader);
void UseShader(Shader shader);
Uniform GetUniformLoc(Shader shader, const std::string& uniform);
void SetUniformMat4(Uniform location, const GLfloat* values);
void SetUniformVec3(Uniform location, const GLfloat* values);

const char* ReadFile(const std::string& path);

}
}

#endif