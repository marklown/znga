#ifndef ZNGA_MESH_H
#define ZNGA_MESH_H

#include <OpenGL/gl3.h>

#include "znga_texture.h"
#include "znga_shader.h"
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

class Mesh 
{
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<GLuint>& indices,
         const Material& material);
    virtual ~Mesh();

    void Render() const;

    Material m_material;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLuint m_numVertices = 0;
    GLuint m_numIndices = 0;
    GLuint m_uniformColor = 0;
};

class MeshInstance
{
public:
    MeshInstance() = default;
    virtual ~MeshInstance() = default;
    MeshInstance(const Mesh* mesh, mat4x4 transform);

    void Render() const;

private:
    const Mesh* m_mesh = nullptr;
    mat4x4 m_transform;
    GLuint m_uniformTransform = 0;
};

} // namespace Graphics
} // namespace Znga

#endif