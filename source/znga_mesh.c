
#include "znga_mesh.h"

znga_mesh_t znga_mesh_create(const znga_vertex_t* vertices, GLuint num_vertices,
                             const GLuint* indices, GLuint num_indices,
                             znga_material_t material)
{
    znga_mesh_t mesh;
    mesh.material = material;
    mesh.num_vertices = num_vertices;
    mesh.num_indices = num_indices;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(znga_vertex_t), &vertices[0], GL_STATIC_DRAW);

    if (num_indices > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(znga_vertex_t), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(znga_vertex_t), (void*)(sizeof(GLfloat) * 3));

    // vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(znga_vertex_t), (void*)(sizeof(GLfloat) * 6));

    glBindVertexArray(0);
    
    return mesh;
}

void znga_mesh_draw(znga_mesh_t* mesh)
{
    if (!mesh) 
    {
        return;
    }

    znga_material_t* material = &mesh->material;

    glUseProgram(material->shader.id);

    znga_shader_set_uniform_vec3(material->shader.loc_u_object_color,
                                 (GLfloat*)material->color);

    glBindVertexArray(mesh->vao);
    if (mesh->num_indices > 0)
    {
        glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
    }
    glBindVertexArray(0);
}