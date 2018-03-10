
#include <GLFW/glfw3.h>
#include "znga.h"
#include "linmath.h"
#include "znga_shader.h"
#include "znga_texture.h"
#include "znga_mesh.h"
#include "znga_cube.h"
#include "znga_model.h"
#include <stdio.h>

typedef struct znga_light_t
{
    vec3 position;
    vec3 color;
} znga_light_t;

static GLFWwindow* window = NULL;

static void glfw_error_callback(int error, const char* desc)
{
    fprintf(stderr, "Error: %s\n", desc);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char* argv[])
{
    printf("znga version %s\n", ZNGA_VERSION);

    if (!glfwInit())
    {
        printf("Failed to initialize glfw\n");
        return 0;
    }

    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    const int width = 1920;
    const int height = 1080;

    window = glfwCreateWindow(width, height, "znga", NULL, NULL);
    if (!window) 
    {
        printf("Failed to create glfw window\n");
        return 0;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    znga_shader_t shader = znga_create_shader(
        "/Users/markl/Dev/znga/shaders/flat.vert",
        "/Users/markl/Dev/znga/shaders/flat.frag"
    );

    znga_texture_t diffuse_map = znga_create_texture(
        "/Users/markl/Dev/znga/textures/brickwall.jpg", DIFFUSE_MAP);

    znga_texture_t normal_map = znga_create_texture(
        "/Users/markl/Dev/znga/textures/brickwall_normal.jpg", NORMAL_MAP);

    znga_material_t material;
    material.diffuse_map = diffuse_map;
    material.normal_map = normal_map;

    //znga_mesh_t cube = znga_create_mesh(cube_vertices, 36, 0, 0, material);
    znga_model_t cube = znga_create_model("/Users/markl/Dev/znga/models/cube.obj");

    glUseProgram(shader);

    mat4x4 projection, model, view;
    mat4x4_identity(projection);
    mat4x4_identity(model);
    mat4x4_identity(view);

    mat4x4_perspective(projection, 45.0f * 3.14f / 180.0f, (float)width/(float)height, 0.1f, 100.0f);
    mat4x4_translate(view, 0, 0, -10.0f);
    mat4x4_translate(model, 0, 0, 0);
    //mat4x4_rotate_X(model, model, 35.0f * 3.14f / 180.f);
    mat4x4_rotate_Y(model, model, 25.0f * 3.14f / 180.f);
    //mat4x4_scale(model, model, .1f);

    znga_light_t light_1 = {{-0.5f, 0.0f, 0.75f}, {1.0f, 0.94f, 0.88f}};
    vec4 color = {0.5f, 0.5f, 0.5f, 1.0f};

    int projection_loc = glGetUniformLocation(shader, "u_projection");
    int view_loc = glGetUniformLocation(shader, "u_view");
    int model_loc = glGetUniformLocation(shader, "u_model");

    int light_pos_loc = glGetUniformLocation(shader, "light_pos");
    int light_col_loc = glGetUniformLocation(shader, "light_color");
    int object_col_loc = glGetUniformLocation(shader, "object_color");

//    int diffuse_map_loc = glGetUniformLocation(shader, "u_diffuse_map");
//    int normal_map_loc = glGetUniformLocation(shader, "u_normal_map");
//    int num_lights_loc = glGetUniformLocation(shader, "u_num_lights");
//    int light_1_position_loc = glGetUniformLocation(shader, "u_light[0].position");
//    int light_1_color_loc = glGetUniformLocation(shader, "u_light[0].color");
//    int light_ambient_loc = glGetUniformLocation(shader, "u_ambient_light_strength");
//    int color_loc = glGetUniformLocation(shader, "u_color");
//    int normal_map_enabled_loc = glGetUniformLocation(shader, "u_normal_map_enabled");

    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (GLfloat*)projection);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, (GLfloat*)model);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, (GLfloat*)view);

    vec3 light_pos = {0.0f, 0.0f, 5.0f};
    vec3 light_col = {.75f, .75f, .75f};
    vec3 object_col = {1.0f, 0.5f, 0.31f};

    glUniform3fv(light_pos_loc, 1, (GLfloat*)light_pos);
    glUniform3fv(light_col_loc, 1, (GLfloat*)light_col);
    glUniform3fv(object_col_loc, 1, (GLfloat*)object_col);

//    glUniform1i(diffuse_map_loc, 0);
//    glUniform1i(normal_map_loc, 1);
//    glUniform3fv(light_1_position_loc, 1, (GLfloat*)light_1.position);
//    glUniform3fv(light_1_color_loc, 1, (GLfloat*)light_1.color);
//    glUniform1f(light_ambient_loc, 0.075f);
//    glUniform4fv(color_loc, 1, (GLfloat*)color);
//    glUniform1i(num_lights_loc, 1);
//    glUniform1i(normal_map_enabled_loc, 0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4_rotate_Y(model, model, 0.005f * 45.0f * 3.14f / 180.f);
        mat4x4_rotate_X(model, model, 0.005f * 45.0f * 3.14f / 180.f);
        mat4x4_rotate_Z(model, model, 0.005f * 45.0f * 3.14f / 180.f);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, (GLfloat*)model);

#if 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map.id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal_map.id);
#endif

        znga_draw_mesh(&cube.meshes[0]);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            printf("OpenGL error: %u\n", err);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    znga_free_model(&cube);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}