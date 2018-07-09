
#include <GLFW/glfw3.h>
#include "Znga.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "World.h"
#include "Model.h"
#include "Terrain.h"
#include "linmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

using namespace Znga::Graphics;

// time
float time_delta = 0.0f;
float time_last = 0.0f;

// camera system
vec3 camera_pos = {CHUNK_SIZE_X/2.0f, CHUNK_SIZE_Y+10.0f, CHUNK_SIZE_Z+20.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};
float camera_last_x = 1920.0f / 2.0f;
float camera_last_y = 1080.0f / 2.0f;
float camera_yaw = -90.0f;
float camera_pitch = 0.0f;
bool first_mouse = true;
bool camera_free = true;

bool fill_mode_line = false;

extern float* terrain_height;
const unsigned int terrain_size = 256;

static GLFWwindow* window = NULL;

World world;

static void glfw_error_callback(int error, const char* desc)
{
    fprintf(stderr, "Error: %s\n", desc);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        if (!fill_mode_line) {
            fill_mode_line = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            fill_mode_line = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        if (!camera_free) {
            camera_free = true;
        } else {
            camera_free = false;
        }
    }
}

static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    if (first_mouse) {
        camera_last_x = x_pos;
        camera_last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - camera_last_x;
    float y_offset = camera_last_y - y_pos;
    camera_last_x = x_pos;
    camera_last_y = y_pos;

    float sensitivity = 0.1;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    camera_yaw += x_offset;
    camera_pitch += y_offset;

    if (camera_pitch > 89.0f) {
        camera_pitch = 89.0f;
    } else if (camera_pitch < -89.0f) {
        camera_pitch = -89.0f;
    }

    float x = cos(RAD(camera_yaw)) * cos(RAD(camera_pitch));
    float y = sin(RAD(camera_pitch));
    float z = sin(RAD(camera_yaw)) * cos(RAD(camera_pitch));
    vec3 front = {x, y, z};
    vec3_norm(front, front);
    memcpy(camera_front, front, sizeof(vec3));
}

static void process_input(GLFWwindow* window)
{
    const float camera_speed = 50.0f * time_delta;
    vec3 tmp;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3_scale(tmp, camera_front, camera_speed);
        vec3_add(camera_pos, camera_pos, tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3_scale(tmp, camera_front, camera_speed);
        vec3_sub(camera_pos, camera_pos, tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3_mul_cross(tmp, camera_front, camera_up);
        vec3_norm(tmp, tmp);
        vec3_scale(tmp, tmp, camera_speed);
        vec3_sub(camera_pos, camera_pos, tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3_mul_cross(tmp, camera_front, camera_up);
        vec3_norm(tmp, tmp);
        vec3_scale(tmp, tmp, camera_speed);
        vec3_add(camera_pos, camera_pos, tmp);
    }

    if (!camera_free) {
        float x = camera_pos[0];
        float z = camera_pos[2];
        float y = terrain_height[(int)x * terrain_size + (int)z] + 1;
        camera_pos[1] = y;
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

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    const int width = 1920;
    const int height = 1080;

    window = glfwCreateWindow(width, height, "znga", NULL, NULL);
    if (!window) {
        printf("Failed to create glfw window\n");
        return 0;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    //glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClearColor(0.196078f, 0.6f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    mat4x4 projection, view;
    mat4x4_identity(projection);
    mat4x4_identity(view);
    mat4x4_perspective(projection, 45.0f * 3.14f / 180.0f, (float)width/(float)height, 0.1f, 100000.0f);

    Shader shader = LoadShader(
        "/Users/markl/Dev/znga/shaders/cube.vert",
        "/Users/markl/Dev/znga/shaders/cube.frag"
    );

    Uniform uniformModel = GetUniformLoc(shader, "u_model");
    Uniform uniformView = GetUniformLoc(shader, "u_view");
    Uniform uniformProjection = GetUniformLoc(shader, "u_projection");
    Uniform uniformTimeOfDay = GetUniformLoc(shader, "u_time_of_day");

    UseShader(shader);

    SetUniformMat4(uniformProjection, (GLfloat*)projection);
    SetUniformMat4(uniformView, (GLfloat*)view);

    mat4x4 worldTransform;
    mat4x4_identity(worldTransform);

    world.Init();
    world.Generate();
//    world.PlacePointlight(4*CHUNK_SIZE_X-1, CHUNK_SIZE_Y-1, 4*CHUNK_SIZE_Z-1);
//    world.PlacePointlight(2*CHUNK_SIZE_X-1, CHUNK_SIZE_Y-1, 6*CHUNK_SIZE_Z-1);
//    world.PlacePointlight(6*CHUNK_SIZE_X-1, CHUNK_SIZE_Y-1, 2*CHUNK_SIZE_Z-1);
    world.Update();

    SetUniform1f(uniformTimeOfDay, 1.0f);


    while (!glfwWindowShouldClose(window)) {
        float time_current = glfwGetTime();
        time_delta = time_current - time_last;
        time_last = time_current;

        process_input(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 center;
        vec3_add(center, camera_pos, camera_front);
        mat4x4_look_at(view, camera_pos, center , camera_up);
        SetUniformMat4(uniformView, (GLfloat*)view);

        SetUniformMat4(uniformModel, (GLfloat*)worldTransform);

        world.Render();

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            printf("OpenGL error: %u\n", err);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}


/*
        mat4x4_rotate_X(model_instance_1.transform,
                        model_instance_1.transform,
                        0.01f * RAD(45.0f));
        mat4x4_rotate_Z(model_instance_2.transform,
                        model_instance_2.transform,
                        0.01f * RAD(75.0f));
        mat4x4_rotate_Y(model_instance_3.transform,
                        model_instance_3.transform,
                        0.01f * RAD(15.0f));
        znga_model_draw_instance(&model_instance_1);
        znga_model_draw_instance(&model_instance_2);
        znga_model_draw_instance(&model_instance_3);
*/


/*
    Model* cube = new Model("/Users/markl/Dev/znga/models/cube.obj");
    cube.meshes[0].material.shader = shader;
    vec3 color = {1.0f, 0.5f, 0.31f};
    memcpy(cube.meshes[0].material.color, color, sizeof(vec3));
    mat4x4 transform;
    mat4x4_identity(transform);
    mat4x4_translate(transform, 32.0f, 6.0f, 32.0f);
    znga_model_instance_t model_instance_1 = znga_model_create_instance(&cube, transform);
    mat4x4_translate(transform, 38.0f, 4.0f, 38.0f);
    znga_model_instance_t model_instance_2 = znga_model_create_instance(&cube, transform);
    mat4x4_translate(transform, 42.0f, 2.0f, 42.0f);
    znga_model_instance_t model_instance_3 = znga_model_create_instance(&cube, transform);
*/
