
#include <GLFW/glfw3.h>
#include "znga.h"
#include "linmath.h"
#include "znga_mesh.h"
#include "znga_cube.h"
#include "znga_shader.h"

#include <stdio.h>

struct znga_light_t
{
    vec3 position;
    vec3 color;
};

static GLFWwindow* window = nullptr;

static void glfw_error_callback(int error, const char* desc)
{
    fprintf(stderr, "Error: %s\n", desc);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char* argv[])
{
    printf("znga version %s\n", ZNGA_VERSION);

    if (!glfwInit()) {
        printf("Failed to initialize glfw\n");
        return 0;
    }

    glfwSetErrorCallback(glfw_error_callback);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 600, "znga", nullptr, nullptr);
    if (!window) {
        printf("Failed to create glfw window\n");
        return 0;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    znga_cube_t cube = znga_create_cube();
    znga_shader_t shader = znga_create_shader(
        "/Users/markl/Dev/znga/shaders/basic.vert",
        "/Users/markl/Dev/znga/shaders/basic.frag"
    );

    glUseProgram(shader);

    mat4x4 projection, model, view;
    mat4x4_identity(projection);
    mat4x4_identity(model);
    mat4x4_identity(view);

    mat4x4_perspective(projection, 45.0f * 3.14f / 180.0f, 800.0f/ 600.0f, 0.1f, 100.0f);
    mat4x4_translate(view, 0, 0, -6.0f);
    mat4x4_translate(model, 0, 0, 0.0f);
    mat4x4_rotate_X(model, model, 35.0f * 3.14f / 180.f);
    mat4x4_rotate_Y(model, model, 45.0f * 3.14f / 180.f);

    znga_light_t light = {{-1.0f, 0.0f, 3.0f}, {1.f, 1.f, 1.f}};
    vec4 color = {0.5f, 0.5f, 0.5f, 1.0f};

    int projection_loc = glGetUniformLocation(shader, "u_projection");
    int view_loc = glGetUniformLocation(shader, "u_view");
    int model_loc = glGetUniformLocation(shader, "u_model");
    int light_position_loc = glGetUniformLocation(shader, "u_light.position");
    int light_color_loc = glGetUniformLocation(shader, "u_light.color");
    int light_ambient_loc = glGetUniformLocation(shader, "u_ambient_light_strength");
    int color_loc = glGetUniformLocation(shader, "u_color");

    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (GLfloat*)projection);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, (GLfloat*)model);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, (GLfloat*)view);
    glUniform3fv(light_position_loc, 1, (GLfloat*)light.position);
    glUniform3fv(light_color_loc, 1, (GLfloat*)light.color);
    glUniform1f(light_ambient_loc, 0.25f);
    glUniform4fv(color_loc, 1, (GLfloat*)color);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //light.position[0] += 0.005;
        //glUniform3fv(light_position_loc, 1, (GLfloat*)light.position);
        mat4x4_rotate_Y(model, model, 0.005f * 45.0f * 3.14f / 180.f);
        mat4x4_rotate_X(model, model, 0.005f * 45.0f * 3.14f / 180.f);
        mat4x4_rotate_Z(model, model, 0.005f * 45.0f * 3.14f / 180.f);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, (GLfloat*)model);

        znga_draw_mesh(&cube.mesh);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}