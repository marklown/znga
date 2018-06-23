
#include "znga_terrain.h"
#include "open-simplex-noise.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace Znga {
namespace Graphics {

float* terrain_height;

static struct osn_context* ctx;

float GetHeightAt(int x, int z)
{
    // 16, 16
    int octaves = 4;
    float val = 0;
    float feature_size = 16;
    for (int i = 1; i <= octaves; i++) 
    {
        val += (float)open_simplex_noise2(ctx, (float)x / feature_size / i, (float)z / feature_size / i);
    }
    return val;
}

Mesh* CreateSmoothTerrain(unsigned int size, const Shader& shader)
{
    open_simplex_noise(77374, &ctx); //77374

    const unsigned int num_vertices = size * size;
    const unsigned int num_indices = size * size * 6;

    std::vector<Vertex> vertices(num_vertices);
    terrain_height = new float[num_vertices];

    float scale = 10.0;
    float y_scale = 5.0;

    for (unsigned int i = 0; i < size; i++)
    {
        for (unsigned int j = 0; j < size; j++)
        {
            float y = GetHeightAt(i, j);

            unsigned int vertex_index = i * size + j;

            y *= y_scale;
            float x = i * scale;
            float z = j * scale;

            terrain_height[vertex_index] = y;

            vertices[vertex_index].position[0] = x;
            vertices[vertex_index].position[1] = y;
            vertices[vertex_index].position[2] = z;

            unsigned int xx = (x > 0 ? x : 1);
            unsigned int zz = (z > 0 ? z : 1);
            float hl = GetHeightAt(xx - 1, zz);
            float hr = GetHeightAt(xx + 1, zz);
            float hd = GetHeightAt(xx, zz + 1);
            float hu = GetHeightAt(xx, zz - 1);
            vec3 normal = {hl - hr, 2.0f, hd - hu};
            vec3_norm(normal, normal);
            vertices[vertex_index].normal[0] = normal[0];
            vertices[vertex_index].normal[1] = normal[1];
            vertices[vertex_index].normal[2] = normal[2];
        }
    }

    std::vector<GLuint> indices(num_indices);

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

    Material material;
    material.shader = shader;
    vec3 color = {96.0f/255.0f, 128.0f/255.0f, 56.0f/255.0f};
    //vec3 color = {255.0f/255.0f, 250.0f/255.0f, 250.0f/255.0f};
    memcpy(material.color, color, sizeof(vec3));

    Mesh* mesh = new Mesh(vertices, indices, material);

    return mesh;
}

Mesh* CreateFlatTerrain(unsigned int size, const Shader& shader)
{
    open_simplex_noise(77374, &ctx);

    const unsigned int num_vertices = size * size * 6;


    std::vector<Vertex> vertices(num_vertices);
    terrain_height = new float[num_vertices];

    unsigned int index = 0;

    float scale = 10.0;
    float y_scale = 5.0;

    for (unsigned int i = 0; i < size; i++)
    {
        for (unsigned int j = 0; j < size; j++)
        {
            float y0 = GetHeightAt(i + 0, j + 0) * y_scale;
            float y1 = GetHeightAt(i + 1, j + 0) * y_scale;
            float y2 = GetHeightAt(i + 0, j + 1) * y_scale;
            float y3 = GetHeightAt(i + 1, j + 0) * y_scale;
            float y4 = GetHeightAt(i + 1, j + 1) * y_scale;
            float y5 = GetHeightAt(i + 0, j + 1) * y_scale;

            float x = i * scale;
            float z = j * scale;

            vertices[index + 0].position[0] = x;
            vertices[index + 0].position[1] = y0;
            vertices[index + 0].position[2] = z;

            vertices[index + 1].position[0] = x + 1 * scale;
            vertices[index + 1].position[1] = y1;
            vertices[index + 1].position[2] = z;

            vertices[index + 2].position[0] = x;
            vertices[index + 2].position[1] = y2;
            vertices[index + 2].position[2] = z + 1 * scale;

            vertices[index + 3].position[0] = x + 1 * scale;
            vertices[index + 3].position[1] = y3;
            vertices[index + 3].position[2] = z;

            vertices[index + 4].position[0] = x + 1 * scale;
            vertices[index + 4].position[1] = y4;
            vertices[index + 4].position[2] = z + 1 * scale;

            vertices[index + 5].position[0] = x;
            vertices[index + 5].position[1] = y5;
            vertices[index + 5].position[2] = z + 1 * scale;

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

            index += 6;
        }
    }

    Material material;
    material.shader = shader;
    vec3 color = {96.0f/255.0f, 128.0f/255.0f, 56.0f/255.0f};
    //vec3 color = {255.0f/255.0f, 250.0f/255.0f, 250.0f/255.0f};
    memcpy(material.color, color, sizeof(vec3));

    std::vector<GLuint> indices;
    Mesh* mesh = new Mesh(vertices, indices, material);

    return mesh;
}

}
}