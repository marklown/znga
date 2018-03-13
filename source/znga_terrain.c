
#include "znga_terrain.h"
#include "znga_noise.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

float* terrain_height;

float znga_terrain_get_height_at(int x, int z)
{
    return perlin2d(x, z, .1, 1);
}

znga_mesh_t znga_terrain_create(unsigned int size)
{
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