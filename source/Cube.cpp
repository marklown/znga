#include "Cube.h"
#include <random>
#include <queue>

namespace Znga {
namespace Graphics {


void CreateMeshForChunk(World& world, Chunk& chunk)
{
    std::vector<Vertex> vertices;
    vertices.reserve(36 * CHUNK_LIST_SIZE);

    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {


                int x = i + chunk.world_pos[0];
                int y = j + chunk.world_pos[1];
                int z = k + chunk.world_pos[2];

                int index = Flatten(i, j, k, CHUNK_SIZE);

                if (AIR == chunk.blocks[index]) {
                    continue;
                }

                if (x > 0) {
                    // Left neighbor
                    if (AIR == GetBlock(world, x - 1, y, z)) {
                        GLfloat torch = (GLfloat)GetTorchlight(world, x - 1, y, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        LeftV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    LeftV(vertices, x, y, z, light_map);
                }

                if (x < WORLD_SIZE * CHUNK_SIZE - 1) {
                    // Right neighbor
                    if (AIR == GetBlock(world, x + 1, y, z)) {
                        GLfloat torch = (GLfloat)GetTorchlight(world, x + 1, y, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        RightV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    RightV(vertices, x, y, z, light_map);
                }

                if (y > 0) {
                    // Bottom neighbor
                    if (AIR == GetBlock(world, x, y - 1, z)) {
                        GLfloat torch = (GLfloat)GetTorchlight(world, x, y - 1, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        BottomV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    BottomV(vertices, x, y, z, light_map);
                }
                
                if (y < WORLD_SIZE * CHUNK_SIZE - 1) {
                    // Top neighbor
                    if (AIR == GetBlock(world, x, y + 1, z)) {
                        GLfloat torch = (GLfloat)GetTorchlight(world, x, y + 1, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        TopV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    TopV(vertices, x, y, z, light_map);
                }

                if (z > 0) {
                    // Back neighbor
                    if (AIR == GetBlock(world, x, y, z - 1)) {
                        GLfloat torch = (GLfloat)GetTorchlight(world, x, y, z - 1);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        BackV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    BackV(vertices, x, y, z, light_map);
                }

                if (z < WORLD_SIZE * CHUNK_SIZE - 1) {
                    // Front neighbor
                    if (AIR == GetBlock(world, x, y, z + 1)) {
                        GLfloat torch = (GLfloat)GetTorchlight(world, x, y, z + 1);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        FrontV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    FrontV(vertices, x, y, z, light_map);
                }
            }
        }
    }

    chunk.mesh = CreateMesh(vertices);
}

void GenerateWorld(World& world)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned int> dist(0, 3);

    for (unsigned int i = 0; i < WORLD_SIZE; i++) {
        for (unsigned int j = 0; j < WORLD_SIZE; j++) {
            for (unsigned int k = 0; k < WORLD_SIZE; k++) {
                Chunk& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                memset(chunk.sun_light, 0, sizeof(chunk.sun_light));
                memset(chunk.torch_light, 0, sizeof(chunk.torch_light));
                chunk.world_pos[0] = i * CHUNK_SIZE;
                chunk.world_pos[1] = j * CHUNK_SIZE;
                chunk.world_pos[2] = k * CHUNK_SIZE;
                for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
                    for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
                        for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
                            if (y == CHUNK_SIZE - 1) {
                                chunk.blocks[Flatten(x, y, z, CHUNK_SIZE)] = AIR;
                            } else {
                                unsigned int type = dist(generator);
                                //type = DIRT;
                                chunk.blocks[Flatten(x, y, z, CHUNK_SIZE)] = (BlockType)type;
                            }
                        }
                    }
                }
            }
        }
    }
}

void RenderWorld(World& world)
{
    for (unsigned int i = 0; i < WORLD_SIZE; i++) {
        for (unsigned int j = 0; j < WORLD_SIZE; j++) {
            for (unsigned int k = 0; k < WORLD_SIZE; k++) {
                const Chunk& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                RenderMesh(chunk.mesh);
            }
        }
    }
}

void UpdateWorld(World& world)
{
    for (unsigned int i = 0; i < WORLD_SIZE; i++) {
        for (unsigned int j = 0; j < WORLD_SIZE; j++) {
            for (unsigned int k = 0; k < WORLD_SIZE; k++) {
                Chunk& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                CreateMeshForChunk(world, chunk);
            }
        }
    }
}

void PlaceTorch(World& world, unsigned int wx, unsigned int wy, unsigned int wz)
{
    std::queue<LightNode> queue;

    SetTorchlight(world, wx, wy, wz, 14);

    unsigned int index = Flatten(wx, wy, wz, CHUNK_SIZE);
    queue.emplace(wx, wy, wz, world.chunks[index]);

    while (!queue.empty()) {
        LightNode& node = queue.front();
        Chunk& chunk = node.chunk;
        queue.pop();
        unsigned int x = node.x;
        unsigned int y = node.y;
        unsigned int z = node.z;

        Light light_level = GetTorchlight(world, x, y, z);

        if (x > 0) {
            // Left neighbor
            if ((GetBlock(world, x - 1, y, z) < DIRT) &&
                (GetTorchlight(world, x - 1, y, z) + 2 <= light_level)) {
                    SetTorchlight(world, x - 1, y, z, light_level - 1);
                    queue.emplace(x - 1, y, z, GetChunk(world, x - 1, y, z));
            }
        }

        if (x < WORLD_SIZE * CHUNK_SIZE - 1) {
            // Right neighbor
            if ((GetBlock(world, x + 1, y, z) < DIRT) &&
                (GetTorchlight(world, x + 1, y, z) + 2 <= light_level)) {
                    SetTorchlight(world, x + 1, y, z, light_level - 1);
                    queue.emplace(x + 1, y, z, GetChunk(world, x + 1, y, z));
            }
        }

        if (y > 0) {
            // Bottom neighbor
            if ((GetBlock(world, x, y - 1, z) < DIRT) &&
                (GetTorchlight(world, x, y - 1, z) + 2 <= light_level)) {
                    SetTorchlight(world, x, y - 1, z, light_level - 1);
                    queue.emplace(x, y - 1, z, GetChunk(world, x, y - 1, z));
            }
        }
        
        if (y < WORLD_SIZE * CHUNK_SIZE - 1) {
            // Top neighbor
            if ((GetBlock(world, x, y + 1, z) < DIRT) &&
                (GetTorchlight(world, x, y + 1, z) + 2 <= light_level)) {
                    SetTorchlight(world, x, y + 1, z, light_level - 1);
                    queue.emplace(x, y + 1, z, GetChunk(world, x, y + 1, z));
            }
        }

        if (z > 0) {
            // Back neighbor
            if ((GetBlock(world, x, y, z - 1) < DIRT) &&
                (GetTorchlight(world, x, y, z - 1) + 2 <= light_level)) {
                    SetTorchlight(world, x, y, z - 1, light_level - 1);
                    queue.emplace(x, y, z - 1, GetChunk(world, x, y, z - 1));
            }
        }

        if (z < WORLD_SIZE * CHUNK_SIZE - 1) {
            // Front neighbor
            if ((GetBlock(world, x, y, z + 1) < DIRT) &&
                (GetTorchlight(world, x, y, z + 1) + 2 <= light_level)) {
                    SetTorchlight(world, x, y, z + 1, light_level - 1);
                    queue.emplace(x, y, z + 1, GetChunk(world, x, y, z + 1));
            }
        }
    }
}


}
}
