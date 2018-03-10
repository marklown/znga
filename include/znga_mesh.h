#ifndef ZNGA_MESH_H
#define ZNGA_MESH_H

#include <OpenGL/gl3.h>
#include "znga_texture.h"
#include "znga_shader.h"

typedef struct znga_material_t
{
    znga_shader_t shader;
    znga_texture_t diffuse_map;
    znga_texture_t specular_map;
    znga_texture_t normal_map;
} znga_material_t;

typedef struct znga_vertex_t
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tex_coord[2];
} znga_vertex_t;

typedef struct znga_mesh_t
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint num_vertices;
    GLuint num_indices;
    znga_material_t material;
} znga_mesh_t;

znga_mesh_t znga_create_mesh(const znga_vertex_t* vertices, GLuint num_vertices,
                             const GLuint* indices, GLuint num_indices,
                             znga_material_t material);

void znga_draw_mesh(znga_mesh_t* mesh);

#endif