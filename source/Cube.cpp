#include "Cube.h"
#include <random>
#include <queue>

namespace Znga {
namespace Graphics {

Chunk* World::GetChunk(const Key& key)
{
    auto it = chunks.find(key);
    if (it != chunks.end()) {
        return it->second;
    }
    return nullptr;
}

Chunk* World::GetChunk(int wx, int wy, int wz)
{
    int x = wx / CHUNK_SIZE;
    int y = wy / CHUNK_SIZE;
    int z = wz / CHUNK_SIZE;
    return GetChunk({x, y, z});
}

Block World::GetBlock(int wx, int wy, int wz)
{
    Chunk* chunk = GetChunk(wx, wy, wz);
    if (!chunk) return AIR;
    int x = wx  % CHUNK_SIZE;
    int y = wy  % CHUNK_SIZE;
    int z = wz  % CHUNK_SIZE;
    return chunk->blocks[x][y][z];
}

void World::UpdateMesh(Chunk* chunk)
{
    if (!chunk) return;
    std::vector<Vertex> vertices;
    vertices.reserve(36 * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {

                int x = i + chunk->world_pos[0];
                int y = j + chunk->world_pos[1];
                int z = k + chunk->world_pos[2];

                if (AIR == chunk->blocks[i][j][k]) {
                    continue;
                }

                if (x > 0) {
                    // Left neighbor
                    if (AIR == GetBlock(x - 1, y, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x - 1, y, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        LeftV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    LeftV(vertices, x, y, z, light_map);
                }

                if (x < WORLD_SIZE_X * CHUNK_SIZE - 1) {
                    // Right neighbor
                    if (AIR == GetBlock(x + 1, y, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x + 1, y, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        RightV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    RightV(vertices, x, y, z, light_map);
                }

                if (y > 0) {
                    // Bottom neighbor
                    if (AIR == GetBlock(x, y - 1, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y - 1, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        BottomV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    BottomV(vertices, x, y, z, light_map);
                }
                
                if (y < WORLD_SIZE_Y * CHUNK_SIZE - 1) {
                    // Top neighbor
                    if (AIR == GetBlock(x, y + 1, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y + 1, z);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        TopV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    TopV(vertices, x, y, z, light_map);
                }

                if (z > 0) {
                    // Back neighbor
                    if (AIR == GetBlock(x, y, z - 1)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y, z - 1);
                        vec4 light_map = {torch, 0.f, 0.f, 0.f};
                        BackV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0.2, 0, 0, 0};
                    BackV(vertices, x, y, z, light_map);
                }

                if (z < WORLD_SIZE_Z * CHUNK_SIZE - 1) {
                    // Front neighbor
                    if (AIR == GetBlock(x, y, z + 1)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y, z + 1);
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

    chunk->mesh = CreateMesh(vertices);
}

Chunk* World::AddChunk(const Key& key)
{
    Chunk* chunk = GetChunk(key);
    if (chunk) return chunk;
    chunk = new Chunk();
    chunks[key] = chunk;
    return chunk;
}

Chunk* World::AddChunk(int wx, int wy, int wz)
{
    int i = wx / CHUNK_SIZE;
    int j = wy / CHUNK_SIZE;
    int k = wz / CHUNK_SIZE;
    return AddChunk({i, j, k});
}

void World::Generate()
{
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned int> dist(0, 3);

    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            for (int k = 0; k < WORLD_SIZE_Z; k++) {

                Chunk* chunk = AddChunk({i, j, k});

                memset(chunk->blocks, 0, sizeof(chunk->pointlight));
                memset(chunk->sunlight, 0, sizeof(chunk->sunlight));
                memset(chunk->pointlight, 0, sizeof(chunk->pointlight));

                chunk->world_pos[0] = i * CHUNK_SIZE;
                chunk->world_pos[1] = j * CHUNK_SIZE;
                chunk->world_pos[2] = k * CHUNK_SIZE;

                for (int x = 0; x < CHUNK_SIZE; x++) {
                    for (int y = 0; y < CHUNK_SIZE; y++) {
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            if (y == CHUNK_SIZE - 1){
                                chunk->blocks[x][y][z] = AIR;
                            } else {
                                chunk->blocks[x][y][z] = (Block)dist(generator);
                            }
                        }
                    }
                }
            }
        }
    }
}

void World::Render()
{
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            for (int k = 0; k < WORLD_SIZE_Z; k++) {
                Chunk* chunk = GetChunk({i, j, k});
                if (chunk) {
                    RenderMesh(chunk->mesh);
                }
            }
        }
    }
}

void World::Update()
{
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            for (int k = 0; k < WORLD_SIZE_Z; k++) {
                Chunk* chunk = GetChunk({i, j, k});
                if (chunk) {
                    UpdateMesh(chunk);
                }
            }
        }
    }
}

Light World::GetPointlight(int wx, int wy, int wz)
{
    Chunk* chunk = GetChunk(wx, wy, wz);
    if (!chunk) return 0;
    int x = wx % CHUNK_SIZE;
    int y = wy % CHUNK_SIZE;
    int z = wz % CHUNK_SIZE;
    return chunk->pointlight[x][y][z];
}

void World::SetPointlight(int wx, int wy, int wz, Light value)
{
    Chunk* chunk = GetChunk(wx, wy, wz);
    if (!chunk) return;
    int x = wx % CHUNK_SIZE;
    int y = wy % CHUNK_SIZE;
    int z = wz % CHUNK_SIZE;
    chunk->pointlight[x][y][z] = value;
}

void World::PlacePointlight(int wx, int wy, int wz)
{
    std::queue<LightNode> queue;

    Chunk* chunk = GetChunk(wx, wy, wz);
    if (!chunk) return;

    SetPointlight(wx, wy, wz, 14);

    queue.emplace(wx, wy, wz);

    while (!queue.empty()) {
        LightNode& node = queue.front();
        queue.pop();
        int x = node.x;
        int y = node.y;
        int z = node.z;

        Light light_level = GetPointlight(x, y, z);

        if (x > 0) {
            // Left neighbor
            if ((GetBlock(x - 1, y, z) < DIRT) &&
                (GetPointlight(x - 1, y, z) + 2 <= light_level)) {
                    SetPointlight(x - 1, y, z, light_level - 1);
                    queue.emplace(x - 1, y, z);
            }
        }

        if (x < WORLD_SIZE_X * CHUNK_SIZE - 1) {
            // Right neighbor
            if ((GetBlock(x + 1, y, z) < DIRT) &&
                (GetPointlight(x + 1, y, z) + 2 <= light_level)) {
                    SetPointlight(x + 1, y, z, light_level - 1);
                    queue.emplace(x + 1, y, z);
            }
        }

        if (y > 0) {
            // Bottom neighbor
            if ((GetBlock(x, y - 1, z) < DIRT) &&
                (GetPointlight(x, y - 1, z) + 2 <= light_level)) {
                    SetPointlight(x, y - 1, z, light_level - 1);
                    queue.emplace(x, y - 1, z);
            }
        }
        
        if (y < WORLD_SIZE_Y * CHUNK_SIZE - 1) {
            // Top neighbor
            if ((GetBlock(x, y + 1, z) < DIRT) &&
                (GetPointlight(x, y + 1, z) + 2 <= light_level)) {
                    SetPointlight(x, y + 1, z, light_level - 1);
                    queue.emplace(x, y + 1, z);
            }
        }

        if (z > 0) {
            // Back neighbor
            if ((GetBlock(x, y, z - 1) < DIRT) &&
                (GetPointlight(x, y, z - 1) + 2 <= light_level)) {
                    SetPointlight(x, y, z - 1, light_level - 1);
                    queue.emplace(x, y, z - 1);
            }
        }

        if (z < WORLD_SIZE_Z * CHUNK_SIZE - 1) {
            // Front neighbor
            if ((GetBlock(x, y, z + 1) < DIRT) &&
                (GetPointlight(x, y, z + 1) + 2 <= light_level)) {
                    SetPointlight(x, y, z + 1, light_level - 1);
                    queue.emplace(x, y, z + 1);
            }
        }
    }
}


}
}
