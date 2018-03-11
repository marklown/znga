#ifndef ZNGA_SHADER_H
#define ZNGA_SHADER_H

#include <GLFW/glfw3.h>

typedef struct znga_shader_t 
{
    GLuint id;
    GLuint loc_u_model;
    GLuint loc_u_view;
    GLuint loc_u_projection;
    GLuint loc_u_light_pos;
    GLuint loc_u_light_color;
    GLuint loc_u_object_color;
} znga_shader_t;

const char* znga_read_file(const char* path);
znga_shader_t znga_shader_create(const char* vpath, const char* fpath);

void znga_shader_set_uniform_mat4(GLuint location, GLfloat* values);
void znga_shader_set_uniform_vec3(GLuint location, GLfloat* values);

#endif