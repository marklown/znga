#version 330 core

uniform mat4 u_model;
uniform sampler2D u_tex;
uniform sampler2D u_normal_map;
uniform int u_normal_map_enabled;
uniform float u_ambient_light_strength;

#define MAX_LIGHTS 2
uniform struct Light {
    vec3 position;
    vec3 color;
} u_light[MAX_LIGHTS];

uniform int u_num_lights;

uniform vec4 u_color;

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 finalColor;

void main()
{
    for (int i = 0; i < u_num_lights; i++) {

        vec3 normal;
        if (u_normal_map_enabled == 1) {
            normal = texture(u_normal_map, tex_coord).rgb;
            normal = normalize(normal * 2.0 - 1.0);
        } else {
            mat3 normal_matrix = transpose(inverse(mat3(u_model)));
            normal = normalize(normal_matrix * normal);
        }

        // calculate the location of this fragment in world coords
        vec3 frag_position = vec3(u_model * vec4(pos, 1));

        // calculate the vector from this pixels surface to the light source
        vec3 surface_to_light = u_light[i].position - frag_position;

        // calculate the cosine of the angle of incidence
        float brightness = dot(normal, surface_to_light) / (length(surface_to_light) * length(normal));
        brightness = clamp(brightness, 0, 1);

        // calculate final color of the pixel, based on:
        // 1. The angle of incidence (brightness)
        // 2. The color of the light
        // 3. The texture and texture coordinates
        vec4 surface_color = texture(u_tex, tex_coord);
        //finalColor += vec4(brightness * u_light[i].color, 1) * u_color + u_ambient_light_strength * u_color;
        finalColor += vec4((brightness * u_light[i].color + u_ambient_light_strength * vec3(1.0f, 1.0f, 1.0f))
            * surface_color.rgb, surface_color.a);
    }
    if (u_num_lights == 0) {
        vec4 surface_color = texture(u_tex, tex_coord);
        finalColor = vec4(surface_color.rgb, surface_color.a);
    }
}