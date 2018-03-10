#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 pos;
out vec3 normal;
out vec2 tex_coord;
out mat3 TBN;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    pos = a_pos;
    normal = a_normal;
    tex_coord = a_tex_coord;
    vec3 T = normalize(vec3(u_model * vec4(a_tangent, 0.0)));
    vec3 B = normalize(vec3(u_model * vec4(a_bitangent, 0.0)));
    vec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));
    TBN = mat3(T, B, N);
}