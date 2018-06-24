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
};

struct Mesh 
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint numVertices;
    GLuint numIndices;
};

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
Mesh CreateMesh(const std::vector<Vertex>& vertices);
void DeleteMesh(Mesh& mesh);
void RenderMesh(const Mesh& mesh);

} // namespace Graphics
} // namespace Znga

#endif