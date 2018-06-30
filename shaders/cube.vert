#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
// [0] = torch brightness
// [1] = torch color
// [2] = sun brightness
// [3] = sun color
layout (location = 3) in vec4 a_light_map;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 uv;
out vec4 light_map;

void main()
{
    uv = a_uv;
    light_map = a_light_map;
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}