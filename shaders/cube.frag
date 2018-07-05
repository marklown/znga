#version 330 core
uniform sampler2D u_texture;
uniform float u_time_of_day;

in vec2 uv;
in vec4 light_map;

out vec4 final_color;

void main()
{
    //vec4 color = vec4(0, .5, .5, 1.0); 
    vec4 color = texture(u_texture, uv);
    float ambient = 0.1;
    float point = light_map[0]/32.0;
    float sun = light_map[2]*u_time_of_day/14.0;
    vec3 lit = min(1.0, (ambient + point + sun)) * color.rgb;
    //vec3 lit = color.rgb;
    final_color = vec4(lit, color.a);
}