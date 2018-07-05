#ifndef ZNGA_MESH_H
#define ZNGA_MESH_H

#include <OpenGL/gl3.h>

#include "Texture.h"
#include "Shader.h"
#include "linmath.h"

#include <vector>

namespace Znga {
namespace Graphics {

struct Material
{
    Shader shader;
    Texture diffuseMap;
    Texture specularMap;
    Texture normalMap;
    vec3 color;
};

struct Vertex 
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tex_coord[2];
    GLfloat light_map[4];
};

struct Mesh 
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint numVertices = 0;
    GLuint numIndices = 0;
};

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
Mesh CreateMesh(const std::vector<Vertex>& vertices);
void DeleteMesh(Mesh& mesh);
void RenderMesh(const Mesh& mesh);

} // namespace Graphics
} // namespace Znga

#endif