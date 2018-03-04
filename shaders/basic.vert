#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 pos;
out vec3 normal;
out vec2 tex_coord;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    pos = a_pos;
    normal = a_normal;
    tex_coord = a_tex_coord;
}