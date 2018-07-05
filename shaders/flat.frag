#version 330 core

in vec3 pos;
in vec3 normal;
in vec3 view_pos;

//uniform vec3 light_pos;
uniform vec3 u_light_dir;
uniform vec3 u_light_color;
uniform vec3 u_object_color;

out vec4 final_color;

void main()
{
    // ambient
    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * u_light_color;

    // diffuse
    vec3 norm = normalize(normal);
    //vec3 light_dir = normalize(light_pos - pos);
    vec3 light_dir = normalize(-u_light_dir);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color * 0.9;

    // specular
    float specular_strength = 0.0;
    vec3 view_dir = normalize(-view_pos - pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 2);
    vec3 specular = specular_strength * spec * u_light_color;

    // final
    vec3 result = (ambient + diffuse + specular) * u_object_color;
    final_color = vec4(result, 1.0);
}