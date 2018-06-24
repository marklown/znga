
#include "Mesh.h"

namespace Znga {
namespace Graphics {

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
    Mesh mesh;
    mesh.numVertices = vertices.size();
    mesh.numIndices = indices.size();

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (mesh.numIndices > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.numIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
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

    return mesh;
}

Mesh CreateMesh(const std::vector<Vertex>& vertices)
{
    std::vector<GLuint> indices;
    return CreateMesh(vertices, indices);
}

void DeleteMesh(Mesh& mesh)
{
    glDeleteBuffers(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);
    mesh.vao = 0;
    mesh.vbo = 0;
    mesh.ebo = 0;
}

void RenderMesh(const Mesh& mesh)
{
    glBindVertexArray(mesh.vao);
    if (mesh.numIndices > 0) {
        glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
    }
    glBindVertexArray(0);
}

} // namespace Graphics
} // namespace Znga