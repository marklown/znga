
#include "znga_terrain.h"
#include "open-simplex-noise.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

float* terrain_height;

static struct osn_context* ctx;

float znga_terrain_get_height_at(int x, int z)
{
    int octaves = 16;
    float val = 0;
    float feature_size = 24;
    for (int i = 1; i <= octaves; i++) 
    {
        val += (float)open_simplex_noise2(ctx, (float)x / feature_size / i, (float)z / feature_size / i);
    }
    return val;
}

znga_mesh_t znga_terrain_create_smooth(unsigned int size)
{
    open_simplex_noise(77374, &ctx);

    const unsigned int num_vertices = size * size;
    const unsigned int num_indices = size * size * 6;

    znga_vertex_t* vertices = malloc(sizeof(znga_vertex_t) * num_vertices);
    memset(vertices, 0, sizeof(znga_vertex_t) * num_vertices);

    terrain_height = malloc(sizeof(float) * num_vertices);

    for (unsigned int i = 0; i < size; i++)
    {
        for (unsigned int j = 0; j < size; j++)
        {
            float y = znga_terrain_get_height_at(i, j);

            unsigned int vertex_index = i * size + j;

            terrain_height[vertex_index] = y;

            vertices[vertex_index].position[0] = i;
            vertices[vertex_index].position[1] = y;
            vertices[vertex_index].position[2] = j;

            unsigned int x = (i > 0 ? i : 1);
            unsigned int z = (j > 0 ? j : 1);
            float hl = znga_terrain_get_height_at(x - 1, z);
            float hr = znga_terrain_get_height_at(x + 1, z);
            float hd = znga_terrain_get_height_at(x, z + 1);
            float hu = znga_terrain_get_height_at(x, z - 1);
            vec3 normal = {hl - hr, 2.0f, hd - hu};
            vec3_norm(normal, normal);
            vertices[vertex_index].normal[0] = normal[0];
            vertices[vertex_index].normal[1] = normal[1];
            vertices[vertex_index].normal[2] = normal[2];
        }
    }

    GLuint* indices = malloc(sizeof(GLuint) * num_indices);
    memset(indices, 0, sizeof(GLuint) * num_indices);

    unsigned int index = 0;
    for (unsigned int i = 0; i < size - 1; i++)
    {
        for (unsigned int j = 0; j < size - 1; j++) 
        {
            unsigned int offset = i * size + j;
            indices[index + 0] = offset + 0;
            indices[index + 1] = offset + 1;
            indices[index + 2] = offset + size;
            indices[index + 3] = offset + 1;
            indices[index + 4] = offset + size + 1;
            indices[index + 5] = offset + size;
            index += 6;
        }
    }

    znga_material_t material;
    vec3 color = {96.0f/255.0f, 128.0f/255.0f, 56.0f/255.0f};
    memcpy(material.color, color, sizeof(vec3));

    znga_mesh_t mesh = znga_mesh_create(vertices, num_vertices, indices, num_indices, material);

    free(vertices);
    free(indices);

    return mesh;
}

znga_mesh_t znga_terrain_create_flat(unsigned int size)
{
    open_simplex_noise(77374, &ctx);

    const unsigned int num_vertices = size * size * 6;

    znga_vertex_t* vertices = malloc(sizeof(znga_vertex_t) * num_vertices);
    memset(vertices, 0, sizeof(znga_vertex_t) * num_vertices);

    terrain_height = malloc(sizeof(float) * size * size);
    memset(terrain_height, 0, sizeof(float) * size * size);

    unsigned int index = 0;

    for (unsigned int i = 0; i < size; i++)
    {
        for (unsigned int j = 0; j < size; j++)
        {

            float y0 = znga_terrain_get_height_at(i + 0, j + 0);
            float y1 = znga_terrain_get_height_at(i + 1, j + 0);
            float y2 = znga_terrain_get_height_at(i + 0, j + 1);
            float y3 = znga_terrain_get_height_at(i + 1, j + 0);
            float y4 = znga_terrain_get_height_at(i + 1, j + 1);
            float y5 = znga_terrain_get_height_at(i + 0, j + 1);

            vertices[index + 0].position[0] = i;
            vertices[index + 0].position[1] = y0;
            vertices[index + 0].position[2] = j;

            vertices[index + 1].position[0] = i + 1;
            vertices[index + 1].position[1] = y1;
            vertices[index + 1].position[2] = j;

            vertices[index + 2].position[0] = i;
            vertices[index + 2].position[1] = y2;
            vertices[index + 2].position[2] = j + 1;

            vertices[index + 3].position[0] = i + 1;
            vertices[index + 3].position[1] = y3;
            vertices[index + 3].position[2] = j;

            vertices[index + 4].position[0] = i + 1;
            vertices[index + 4].position[1] = y4;
            vertices[index + 4].position[2] = j + 1;

            vertices[index + 5].position[0] = i;
            vertices[index + 5].position[1] = y5;
            vertices[index + 5].position[2] = j + 1;

            vec3 normal;
            vec3 vec1, vec2;
            vec3_sub(vec1, vertices[index + 1].position, vertices[index + 0].position);
            vec3_sub(vec2, vertices[index + 2].position, vertices[index + 0].position);
            vec3_mul_cross(normal, vec2, vec1);
            vec3_norm(normal, normal);
            memcpy(vertices[index + 0].normal, normal, sizeof(vec3));
            memcpy(vertices[index + 1].normal, normal, sizeof(vec3));
            memcpy(vertices[index + 2].normal, normal, sizeof(vec3));

            vec3_sub(vec1, vertices[index + 2].position, vertices[index + 4].position);
            vec3_sub(vec2, vertices[index + 3].position, vertices[index + 4].position);
            vec3_mul_cross(normal, vec2, vec1);
            vec3_norm(normal, normal);
            memcpy(vertices[index + 3].normal, normal, sizeof(vec3));
            memcpy(vertices[index + 4].normal, normal, sizeof(vec3));
            memcpy(vertices[index + 5].normal, normal, sizeof(vec3));

            //unsigned int vertex_index = i * size + j;
            //terrain_height[vertex_index] = y;

            index += 6;
        }
    }

    znga_material_t material;
    vec3 color = {96.0f/255.0f, 128.0f/255.0f, 56.0f/255.0f};
    memcpy(material.color, color, sizeof(vec3));

    znga_mesh_t mesh = znga_mesh_create(vertices, num_vertices, 0, 0, material);

    free(vertices);

    return mesh;
}