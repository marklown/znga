#include "World.h"
#include "noise.h"
#include "Simplex.h"
#include <random>
#include <queue>
#include <chrono>

namespace Znga {
namespace Graphics {

World::World()
{
}

void World::Init()
{
    opensimplex_init(41716);

    m_blockAtlas = new TextureAtlas("/Users/markl/Dev/znga/textures/test.png",
                                    "/Users/markl/Dev/znga/textures/test.json");

    m_dirtBlockInfo = new DirtBlockInfo(*m_blockAtlas);
    m_grassBlockInfo = new GrassBlockInfo(*m_blockAtlas);
    m_sandBlockInfo = new SandBlockInfo(*m_blockAtlas);
    m_stoneBlockInfo = new StoneBlockInfo(*m_blockAtlas);

    m_blockInfoMap[DIRT] = m_dirtBlockInfo;
    m_blockInfoMap[GRASS] = m_grassBlockInfo;
    m_blockInfoMap[SAND] = m_sandBlockInfo;
    m_blockInfoMap[STONE] = m_stoneBlockInfo;
}

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
    if (x < 0 || y < 0 || z < 0) return -1;
    int i = x / CHUNK_SIZE_X;
    int j = y / CHUNK_SIZE_Y;
    int k = z / CHUNK_SIZE_Z;
    return HashFromIndex(i, j, k);
}

int World::HashFromIndex(int i, int j, int k)
{
    //return i + j * CHUNK_SIZE_X + k * CHUNK_SIZE_Y * CHUNK_SIZE_Y;
    return i + j * WORLD_SIZE_Y + k * WORLD_SIZE_Z * CHUNK_SIZE_Z;
}

Block World::GetBlock(int x, int y, int z)
{
    Chunk* chunk = GetChunkByPos(x, y, z);
    if (!chunk) return AIR;
    int i = x % CHUNK_SIZE_X;
    int j = y % CHUNK_SIZE_Y;
    int k = z % CHUNK_SIZE_Z;
    return chunk->blocks[i][j][k];
}

void World::UpdateMesh(Chunk* chunk)
{
    if (!chunk) return;

    PropogateSunlight(chunk);

    std::vector<Vertex> vertices;
    vertices.reserve(36 * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);

    std::vector<FaceInfo> faces;
    std::vector<Vertex> cube;

    const GLfloat ao = 1.8;

    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {

                int x = i + chunk->world_pos[0];
                int y = j + chunk->world_pos[1];
                int z = k + chunk->world_pos[2];

                Block block_type = chunk->blocks[i][j][k];
                if (AIR == block_type) {
                    continue;
                }

                faces.clear();
                faces.reserve(6);

                // Left neighbor
                if (AIR == GetBlock(x-1, y, z)) {
                    faces.emplace_back(LEFT, (GLfloat)GetPointlight(x-1,y,z), (GLfloat)GetSunlight(x-1,y,z)-ao, 0);
                }

                // Right neighbor
                if (AIR == GetBlock(x+1, y, z)) {
                    faces.emplace_back(RIGHT, (GLfloat)GetPointlight(x+1,y,z), (GLfloat)GetSunlight(x+1,y,z)-ao, 0);
                }

                // Bottom neighbor
                if (AIR == GetBlock(x, y-1, z)) {
                    faces.emplace_back(BOTTOM,(GLfloat)GetPointlight(x,y-1,z), (GLfloat)GetSunlight(x,y-1,z), 0);
                }

                if (GetBlock(x,y+6,z) == AIR) {
                    block_type = DIRT;
                }
            
                // Top neighbor
                if (AIR == GetBlock(x, y+1, z)) {
                    faces.emplace_back(TOP, (GLfloat)GetPointlight(x,y+1,z), (GLfloat)GetSunlight(x,y+1,z), 0);
                    if (block_type == DIRT && GetSunlight(x,y+1,z) > 6) {
                        block_type = GRASS;
                    }
                    chunk->blocks[i][j][k] = block_type;
                }

                // Back neighbor
                if (AIR == GetBlock(x, y, z-1)) {
                    faces.emplace_back(BACK, (GLfloat)GetPointlight(x,y,z-1), (GLfloat)GetSunlight(x,y,z-1)-ao, 0);
                }

                // Front neighbor
                if (AIR == GetBlock(x, y, z+1)) {
                    faces.emplace_back(FRONT, (GLfloat)GetPointlight(x,y,z+1), (GLfloat)GetSunlight(x,y,z+1)-ao, 0);
                }

                auto it = m_blockInfoMap.find(block_type);
                if (it != m_blockInfoMap.end()) {
                    cube = MeshFactory::CreateBlockMesh(x, y, z, *it->second, faces);
                    vertices.insert(vertices.end(), cube.begin(), cube.end());
                }
            }
        }
    }

    chunk->mesh = CreateMesh(vertices);
    chunk->has_mesh = true;
}

void World::Generate(Chunk* chunk)
{
    memset(chunk->blocks, 0, sizeof(chunk->pointlight));
    memset(chunk->sunlight, 0, sizeof(chunk->sunlight));
    memset(chunk->pointlight, 0, sizeof(chunk->pointlight));
    float caves, center_falloff, plateau_falloff, density;
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {
                float xf = (float)(i+chunk->world_pos[0])/(float)(CHUNK_SIZE_X*WORLD_SIZE_X);
                float yf = (float)(j+chunk->world_pos[1])/(float)(CHUNK_SIZE_Y*WORLD_SIZE_Y);
                float zf = (float)(k+chunk->world_pos[2])/(float)(CHUNK_SIZE_Z*WORLD_SIZE_Z);
                if (yf <= 0.8) {
                    plateau_falloff = 1.0;
                } else if (0.8 < yf && yf < 0.9){
                    plateau_falloff = 1.0-(yf-0.8)*10.0;
                } else {
                    plateau_falloff = 0.0;
                }
                center_falloff = 0.1 / (pow((xf-0.5)*1.5, 2) + pow((yf-1.0)*0.8, 2) + pow((zf-0.5)*1.5, 2));
                caves = pow(simplex_noise(1, xf*5, yf*5, zf*5), 3);
                density = simplex_noise(5, xf, yf*0.5, zf) * center_falloff * plateau_falloff;
                density *= pow(noise((xf+1)*3.0, (yf+1)*3.0, (zf+1)*3.0)+0.4, 1.8);
                if (caves < 0.5) {
                    density = 0;
                }
                chunk->blocks[i][j][k] = density > 3.0 ? STONE : AIR;
            }
        }
    }
    chunk->has_mesh = false;
    m_updateQueue.push(chunk);
}

void World::Generate()
{
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            for (int k = 0; k < WORLD_SIZE_Z; k++) {
                Chunk* chunk = AddChunkByIndex(i, j, k);
                chunk->world_pos[0] = i * CHUNK_SIZE_X;
                chunk->world_pos[1] = j * CHUNK_SIZE_Y;
                chunk->world_pos[2] = k * CHUNK_SIZE_Z;
                m_genQueue.push(chunk);
            }
        }
    }
}

void World::Render()
{
    glBindTexture(GL_TEXTURE_2D, m_blockAtlas->GetTexture().id);
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            for (int k = 0; k < WORLD_SIZE_Z; k++) {
                Chunk* chunk = GetChunkByIndex(i, j, k);
                if (chunk && chunk->has_mesh) {
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

    SetPointlight(x, y, z, 32);

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
    if (value < 4) value = 4;
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
                SetSunlight(x + i, y + j, z + k, 14);
                queue.emplace(x + i, y + j, z + k);
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
            if (GetBlock(x, y - 1, z) < DIRT) {
                if (GetSunlight(x, y - 1, z) + 2 <= light_level) {
                    SetSunlight(x, y - 1, z, light_level);
                    queue.emplace(x, y - 1, z);
                }
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

void World::ProcessUpdateQueue()
{
    if (m_updateQueue.size() > 0) {
        Chunk* chunk = m_updateQueue.front();
        m_updateQueue.pop();
        UpdateMesh(chunk);
    }
}

void World::ProcessGenQueue()
{
    while (m_genQueue.size() > 0) {
        Chunk* chunk = m_genQueue.front();
        m_genQueue.pop();
        Generate(chunk);
    }
}

bool World::Collides(const Znga::Physics::AABB& other)
{
    Chunk* c = GetChunkByPos(other.xmin, other.ymin, other.zmin);
    if (!c) {
        return false;
    }
    for (int i = 0; i<CHUNK_SIZE_X; i++) {
        for (int j = 0;j<CHUNK_SIZE_Y; j++) {
            for (int k = 0;k<CHUNK_SIZE_Z; k++) {
                int x = c->world_pos[0] + i;
                int y = c->world_pos[1] + j;
                int z = c->world_pos[2] + k;
                Znga::Physics::AABB b = {x-.5f, x+.5f, y-.5f, y+.5f, z-.5f, z+.5f};
                if (c->blocks[i][j][k] > AIR && b.Collides(other)) {
                    return true;
                }
            }
        }
    }
    return false;
}

}
}
