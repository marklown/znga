
#include "znga_mesh.h"

namespace Znga {
namespace Graphics {

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           const Material& material)
    : m_numVertices(vertices.size())
    , m_numIndices(indices.size())
    , m_material(material)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (m_numIndices > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

    // vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));

    glBindVertexArray(0);

    m_uniformColor = m_material.shader.GetUniformLocation("u_object_color");
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Mesh::Render() const
{
    m_material.shader.Enable();
    m_material.shader.SetUniformVec3(m_uniformColor, (GLfloat*)m_material.color);

    glBindVertexArray(m_vao);
    if (m_numIndices > 0) {
        glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    }
    glBindVertexArray(0);
}


MeshInstance::MeshInstance(const Mesh* mesh, mat4x4 transform)
    : m_mesh(mesh)
{
    mat4x4_dup(m_transform, transform);
    if (mesh) {
        m_uniformTransform = m_mesh->m_material.shader.GetUniformLocation("u_model");
    }
}

void MeshInstance::Render() const
{
    if (!m_mesh) {
        return;
    }
    m_mesh->m_material.shader.SetUniformMat4(m_uniformTransform, (GLfloat*)m_transform);
    m_mesh->Render();
}

} // namespace Graphics
} // namespace Znga