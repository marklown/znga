#ifndef ZNGA_MESH_H
#define ZNGA_MESH_H

//#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

struct znga_vertex_t
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tex_coord[2];
};

struct znga_mesh_t
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint num_vertices;
    GLuint num_indices;
};

znga_mesh_t znga_create_mesh(znga_vertex_t vertices[], GLuint num_vertices,
                             GLuint indices[], GLuint num_indices);

void znga_draw_mesh(znga_mesh_t* mesh);

#endif