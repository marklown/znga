#include "Cube.h"
#include <random>
#include <queue>

namespace Znga {
namespace Graphics {

Chunk* World::GetChunk(int hash)
{
    auto it = m_chunks.find(hash);
    if (it != m_chunks.end()) {
        return it->second;
    }
    return nullptr;
}

Chunk* World::GetChunkByPos(int x, int y, int z)
{
    int key = HashFromPos(x, y, z);
    return GetChunk(key);
}

Chunk* World::GetChunkByIndex(int i, int j, int k)
{
    int key = HashFromIndex(i, j, k);
    return GetChunk(key);
}

Chunk* World::AddChunk(int hash)
{
    Chunk* chunk = GetChunk(hash);
    if (chunk) {
        return chunk;
    }
    chunk = new Chunk();
    m_chunks[hash] = chunk;
    return chunk;
}

Chunk* World::AddChunkByPos(int x, int y, int z)
{
    int key = HashFromPos(x, y, z);
    return AddChunk(key);
}

Chunk* World::AddChunkByIndex(int i, int j, int k)
{
    int key = HashFromIndex(i, j, k);
    return AddChunk(key);
}

int World::HashFromPos(int x, int y, int z)
{
    int i = x / CHUNK_SIZE_X;
    int j = y / CHUNK_SIZE_Y;
    int k = z / CHUNK_SIZE_Z;
    return HashFromIndex(i, j, k);
}

int World::HashFromIndex(int i, int j, int k)
{
    return i + j * CHUNK_SIZE_X + k * CHUNK_SIZE_Y * CHUNK_SIZE_Y;
}

Block World::GetBlock(int x, int y, int z)
{
    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) return AIR;
    int i = x  % CHUNK_SIZE_X;
    int j = y  % CHUNK_SIZE_Y;
    int k = z  % CHUNK_SIZE_Z;
    return chunk->blocks[i][j][k];
}

void World::UpdateMesh(Chunk* chunk)
{
    if (!chunk) return;

    PropogateSunlight(chunk);

    std::vector<Vertex> vertices;
    vertices.reserve(36 * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);

    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {

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
                        GLfloat sun = (GLfloat)GetSunlight(x - 1, y, z);
                        vec4 light_map = {torch, 0.f, sun, 0.f};
                        LeftV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0, 0, 0, 0};
                    LeftV(vertices, x, y, z, light_map);
                }

                if (x < WORLD_SIZE_X * CHUNK_SIZE_X - 1) {
                    // Right neighbor
                    if (AIR == GetBlock(x + 1, y, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x + 1, y, z);
                        GLfloat sun = (GLfloat)GetSunlight(x + 1, y, z);
                        vec4 light_map = {torch, 0.f, sun, 0.f};
                        RightV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0, 0, 0, 0};
                    RightV(vertices, x, y, z, light_map);
                }

                if (y > 0) {
                    // Bottom neighbor
                    if (AIR == GetBlock(x, y - 1, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y - 1, z);
                        GLfloat sun = (GLfloat)GetSunlight(x, y - 1, z);
                        vec4 light_map = {torch, 0.f, sun, 0.f};
                        BottomV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0, 0, 0, 0};
                    BottomV(vertices, x, y, z, light_map);
                }
                
                if (y < WORLD_SIZE_Y * CHUNK_SIZE_Y - 1) {
                    // Top neighbor
                    if (AIR == GetBlock(x, y + 1, z)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y + 1, z);
                        GLfloat sun = (GLfloat)GetSunlight(x, y + 1, z);
                        vec4 light_map = {torch, 0.f, sun, 0.f};
                        TopV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0, 0, 0, 0};
                    TopV(vertices, x, y, z, light_map);
                }

                if (z > 0) {
                    // Back neighbor
                    if (AIR == GetBlock(x, y, z - 1)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y, z - 1);
                        GLfloat sun = (GLfloat)GetSunlight(x, y, z - 1);
                        vec4 light_map = {torch, 0.f, sun, 0.f};
                        BackV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0, 0, 0, 0};
                    BackV(vertices, x, y, z, light_map);
                }

                if (z < WORLD_SIZE_Z * CHUNK_SIZE_Z - 1) {
                    // Front neighbor
                    if (AIR == GetBlock(x, y, z + 1)) {
                        GLfloat torch = (GLfloat)GetPointlight(x, y, z + 1);
                        GLfloat sun = (GLfloat)GetSunlight(x, y, z + 1);
                        vec4 light_map = {torch, 0.f, sun, 0.f};
                        FrontV(vertices, x, y, z, light_map);
                    }
                } else {
                    vec4 light_map = {0, 0, 0, 0};
                    FrontV(vertices, x, y, z, light_map);
                }
            }
        }
    }

    chunk->mesh = CreateMesh(vertices);
}

void World::Generate()
{
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned int> dist(0, 100);

    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            for (int k = 0; k < WORLD_SIZE_Z; k++) {

                Chunk* chunk = AddChunkByIndex(i, j, k);

                memset(chunk->blocks, 0, sizeof(chunk->pointlight));
                memset(chunk->sunlight, 0, sizeof(chunk->sunlight));
                memset(chunk->pointlight, 0, sizeof(chunk->pointlight));

                chunk->world_pos[0] = i * CHUNK_SIZE_X;
                chunk->world_pos[1] = j * CHUNK_SIZE_Y;
                chunk->world_pos[2] = k * CHUNK_SIZE_Z;

                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                            unsigned int r = dist(generator);
                            if (y == CHUNK_SIZE_Y - 1){
                                chunk->blocks[x][y][z] = AIR;
                            } else {
                                if (r < 75) {
                                    chunk->blocks[x][y][z] = AIR;
                                } else {
                                    chunk->blocks[x][y][z] = DIRT;
                                }
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
                Chunk* chunk = GetChunkByIndex(i, j, k);
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
                Chunk* chunk = GetChunkByIndex(i, j, k);
                if (chunk) {
                    UpdateMesh(chunk);
                }
            }
        }
    }
}

Light World::GetPointlight(int x, int y, int z)
{
    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) return 0;
    int i = x % CHUNK_SIZE_X;
    int j = y % CHUNK_SIZE_Y;
    int k = z % CHUNK_SIZE_Z;
    return chunk->pointlight[i][j][k];
}

void World::SetPointlight(int x, int y, int z, Light value)
{
    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) return;
    int i = x % CHUNK_SIZE_X;
    int j = y % CHUNK_SIZE_Y;
    int k = z % CHUNK_SIZE_Z;
    chunk->pointlight[i][j][k] = value;
}

void World::PlacePointlight(int x, int y, int z)
{
    std::queue<LightNode> queue;

    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) return;

    SetPointlight(x, y, z, 14);

    queue.emplace(x, y, z);

    while (!queue.empty()) {
        LightNode& node = queue.front();
        queue.pop();
        x = node.x;
        y = node.y;
        z = node.z;

        Light light_level = GetPointlight(x, y, z);

        if (x > 0) {
            // Left neighbor
            if ((GetBlock(x - 1, y, z) < DIRT) &&
                (GetPointlight(x - 1, y, z) + 2 <= light_level)) {
                    SetPointlight(x - 1, y, z, light_level - 1);
                    queue.emplace(x - 1, y, z);
            }
        }

        if (x < WORLD_SIZE_X * CHUNK_SIZE_X - 1) {
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
        
        if (y < WORLD_SIZE_Y * CHUNK_SIZE_Y - 1) {
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

        if (z < WORLD_SIZE_Z * CHUNK_SIZE_Z - 1) {
            // Front neighbor
            if ((GetBlock(x, y, z + 1) < DIRT) &&
                (GetPointlight(x, y, z + 1) + 2 <= light_level)) {
                    SetPointlight(x, y, z + 1, light_level - 1);
                    queue.emplace(x, y, z + 1);
            }
        }
    }
}

Light World::GetSunlight(int x, int y, int z)
{
    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) {
        return 0;
    }
    int i = x % CHUNK_SIZE_X;
    int j = y % CHUNK_SIZE_Y;
    int k = z % CHUNK_SIZE_Z;
    return chunk->sunlight[i][j][k];
}

void World::SetSunlight(int x, int y, int z, Light value)
{
    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) {
        return;
    }
    int i = x % CHUNK_SIZE_X;
    int j = y % CHUNK_SIZE_Y;
    int k = z % CHUNK_SIZE_Z;
    chunk->sunlight[i][j][k] = value;
}

void World::PropogateSunlight(Chunk* chunk)
{
    std::queue<LightNode> queue;

    if (!chunk) return;

    int x = chunk->world_pos[0];
    int y = chunk->world_pos[1];
    int z = chunk->world_pos[2];

    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int k = 0; k < CHUNK_SIZE_Z; k++) {
            int j = CHUNK_SIZE_Y - 1;
            if (GetBlock(x + i, y + j, z + k) < DIRT) {
                SetSunlight(x + i, y + j, z + k, 7);
                queue.emplace(x + i, y + j, z + k);
            } else {
                SetSunlight(x + i, y + j, z + k, 0);
            }
        }
    }

    while (!queue.empty()) {
        LightNode& node = queue.front();
        queue.pop();
        x = node.x;
        y = node.y;
        z = node.z;

        Light light_level = GetSunlight(x, y, z);

        if (x > 0) {
            // Left neighbor
            if ((GetBlock(x - 1, y, z) < DIRT) &&
                (GetSunlight(x - 1, y, z) + 2 <= light_level)) {
                    SetSunlight(x - 1, y, z, light_level - 1);
                    queue.emplace(x - 1, y, z);
            }
        }

        if (x < WORLD_SIZE_X * CHUNK_SIZE_X - 1) {
            // Right neighbor
            if ((GetBlock(x + 1, y, z) < DIRT) &&
                (GetSunlight(x + 1, y, z) + 2 <= light_level)) {
                    SetSunlight(x + 1, y, z, light_level - 1);
                    queue.emplace(x + 1, y, z);
            }
        }

        if (y > 0) {
            // Bottom neighbor
            if ((GetBlock(x, y - 1, z) < DIRT) &&
                (GetSunlight(x, y - 1, z) + 2 <= light_level)) {
                    // Sunlight propogates straight down with no attenuation
                    SetSunlight(x, y - 1, z, light_level /*- 1*/);
                    queue.emplace(x, y - 1, z);
            }
        }
        
        if (y < WORLD_SIZE_Y * CHUNK_SIZE_Y - 1) {
            // Top neighbor
            if ((GetBlock(x, y + 1, z) < DIRT) &&
                (GetSunlight(x, y + 1, z) + 2 <= light_level)) {
                    SetSunlight(x, y + 1, z, light_level - 1);
                    queue.emplace(x, y + 1, z);
            }
        }

        if (z > 0) {
            // Back neighbor
            if ((GetBlock(x, y, z - 1) < DIRT) &&
                (GetSunlight(x, y, z - 1) + 2 <= light_level)) {
                    SetSunlight(x, y, z - 1, light_level - 1);
                    queue.emplace(x, y, z - 1);
            }
        }

        if (z < WORLD_SIZE_Z * CHUNK_SIZE_Z - 1) {
            // Front neighbor
            if ((GetBlock(x, y, z + 1) < DIRT) &&
                (GetSunlight(x, y, z + 1) + 2 <= light_level)) {
                    SetSunlight(x, y, z + 1, light_level - 1);
                    queue.emplace(x, y, z + 1);
            }
        }
    }
}


}
}
