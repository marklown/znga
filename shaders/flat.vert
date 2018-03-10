#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 pos;
out vec3 normal;
out vec3 view_pos;

void main()
{
    pos = vec3(u_model * vec4(a_pos, 1.0));
    normal = mat3(transpose(inverse(u_model))) * a_normal;
    view_pos = vec3(u_view[3]);
    gl_Position = u_projection * u_view * vec4(pos, 1.0);
}