
#include <GLFW/glfw3.h>
#include "Znga.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "World.h"
#include "Model.h"
#include "Terrain.h"
#include "Collision.h"
#include "linmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <thread>
#include <iostream>

using namespace Znga::Graphics;
using namespace Znga::Physics;

// time
float time_delta = 0.0f;
float time_last = 0.0f;

// camera system
struct Camera
{
    vec3 pos = {(CHUNK_SIZE_X*WORLD_SIZE_X)/2, CHUNK_SIZE_Y, (CHUNK_SIZE_Z*WORLD_SIZE_Z)/2};
    vec3 front = {0.0f, 0.0f, 1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    float last_x = 1920.0f / 2.0f;
    float last_y = 1080.0f / 2.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    bool first_mouse = true;
    bool free = true;
};

bool fill_mode_line = false;

extern float* terrain_height;
const unsigned int terrain_size = 256;

static GLFWwindow* window = NULL;

Uniform uniformModel;// = GetUniformLoc(shader, "u_model");
Uniform uniformView;// = GetUniformLoc(shader, "u_view");
Uniform uniformProjection;// = GetUniformLoc(shader, "u_projection");
Uniform uniformTimeOfDay;// = GetUniformLoc(shader, "u_time_of_day");

struct Player
{
    vec3 position;
    AABB aabb = {-.25f, .25f, -1.0f, .25f, -.25f, .25f};
    bool render_collision_mesh = false;
    void SetPosition(vec3 pos)
    {
        memcpy(position, pos, 3 * sizeof(float));
        aabb.xmin = pos[0]-.25f;
        aabb.xmax = pos[0]+.25f;
        aabb.ymin = pos[1]-1.0f;
        aabb.ymax = pos[1]+0.25f;
        aabb.zmin = pos[2]-.25f;
        aabb.zmax = pos[2]+.25f;
    }
    Mesh collision_mesh;
    void Init()
    {
        std::vector<Vertex> v = {
            {aabb.xmin, aabb.ymin, aabb.zmax, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmax, aabb.ymin, aabb.zmax, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmax, aabb.ymax, aabb.zmax, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmin, aabb.ymax, aabb.zmax, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmax, aabb.ymin, aabb.zmin, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmin, aabb.ymin, aabb.zmin, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmin, aabb.ymax, aabb.zmin, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {aabb.xmax, aabb.ymax, aabb.zmin, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        std::vector<GLuint> e = {
            0, 1, 2,
            2, 3, 0,
            1, 4, 7,
            7, 2, 1,
            4, 5, 6,
            6, 7, 4,
            5, 0, 3,
            3, 6, 5,
        };
        collision_mesh = CreateMesh(v, e);
    }
    void RenderCollisionMesh()
    {
        if (render_collision_mesh) {
            mat4x4 transform;
            mat4x4_identity(transform);
            mat4x4_translate(transform, position[0], position[1], position[2]);
            SetUniformMat4(uniformModel, (GLfloat*)transform);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            RenderMesh(collision_mesh);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
        }
    }
};

Camera camera;
Player player;
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
        if (!camera.free) {
            camera.free = true;
        } else {
            camera.free = false;
        }
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (!player.render_collision_mesh) {
            player.render_collision_mesh = true;
        } else {
            player.render_collision_mesh = false;
        }
    }
}

static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    if (camera.first_mouse) {
        camera.last_x = x_pos;
        camera.last_y = y_pos;
        camera.first_mouse = false;
    }

    float x_offset = x_pos - camera.last_x;
    float y_offset = camera.last_y - y_pos;
    camera.last_x = x_pos;
    camera.last_y = y_pos;

    float sensitivity = 0.1;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    camera.yaw += x_offset;
    camera.pitch += y_offset;

    if (camera.pitch > 89.0f) {
        camera.pitch = 89.0f;
    } else if (camera.pitch < -89.0f) {
        camera.pitch = -89.0f;
    }

    float x = cos(RAD(camera.yaw)) * cos(RAD(camera.pitch));
    float y = sin(RAD(camera.pitch));
    float z = sin(RAD(camera.yaw)) * cos(RAD(camera.pitch));
    vec3 front = {x, y, z};
    vec3_norm(front, front);
    memcpy(camera.front, front, sizeof(vec3));
}

static void process_input(GLFWwindow* window)
{
    const float speed = 50.0f * time_delta;
    vec3 tmp;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3_scale(tmp, camera.front, speed);
        vec3_add(camera.pos, camera.pos, tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3_scale(tmp, camera.front, speed);
        vec3_sub(camera.pos, camera.pos, tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3_mul_cross(tmp, camera.front, camera.up);
        vec3_norm(tmp, tmp);
        vec3_scale(tmp, tmp, speed);
        vec3_sub(camera.pos, camera.pos, tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3_mul_cross(tmp, camera.front, camera.up);
        vec3_norm(tmp, tmp);
        vec3_scale(tmp, tmp, speed);
        vec3_add(camera.pos, camera.pos, tmp);
    }

    vec3 last_pos;
    memcpy(last_pos, player.position, sizeof(last_pos));
    player.SetPosition(camera.pos);
    if (world.Collides(player.aabb)) {
        player.SetPosition(last_pos);
        memcpy(camera.pos, player.position, sizeof(camera.pos));
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
    mat4x4_perspective(projection, 45.0f * 3.14f / 180.0f, (float)width/(float)height, 0.1f, 10000.0f);

    Shader shader = LoadShader(
        "/Users/markl/Dev/znga/shaders/cube.vert",
        "/Users/markl/Dev/znga/shaders/cube.frag"
    );

    uniformModel = GetUniformLoc(shader, "u_model");
    uniformView = GetUniformLoc(shader, "u_view");
    uniformProjection = GetUniformLoc(shader, "u_projection");
    uniformTimeOfDay = GetUniformLoc(shader, "u_time_of_day");

    UseShader(shader);

    SetUniformMat4(uniformProjection, (GLfloat*)projection);
    SetUniformMat4(uniformView, (GLfloat*)view);

    mat4x4 worldTransform;
    mat4x4_identity(worldTransform);

    world.Init();
    world.Generate();
//    world.PlacePointlight(CHUNK_SIZE_X/4, CHUNK_SIZE_Y-4, CHUNK_SIZE_Z/2);
//    world.Update();

    SetUniform1f(uniformTimeOfDay, 1.0f);

    player.Init();
    player.SetPosition(camera.pos);

    while (!glfwWindowShouldClose(window)) {
        float time_current = glfwGetTime();
        time_delta = time_current - time_last;
        time_last = time_current;

        process_input(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 center;
        vec3_add(center, camera.pos, camera.front);
        mat4x4_look_at(view, camera.pos, center , camera.up);
        SetUniformMat4(uniformView, (GLfloat*)view);

        SetUniformMat4(uniformModel, (GLfloat*)worldTransform);
        world.ProcessGenQueue();
        world.ProcessUpdateQueue();
        world.Render();

        player.RenderCollisionMesh();

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
