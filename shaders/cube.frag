#version 330 core
uniform sampler2D u_texture;

in vec2 uv;
in vec4 light_map;

out vec4 final_color;

void main()
{
    //vec4 color = vec4(.5, .5, .5, 1.0); 
    vec4 color = vec4(0, .5, .5, 1.0); 
    //vec4 color = texture(u_texture, uv);
    float ambient = 0.2;
    vec3 lit = min(1.0, ambient + (light_map[0] + light_map[2])/14.0) * color.rgb;
    final_color = vec4(lit, color.a);
}