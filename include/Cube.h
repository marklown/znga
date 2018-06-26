
#ifndef ZNGA_CUBE_H
#define ZNGA_CUBE_H

#include "Mesh.h"

namespace Znga {
namespace Graphics {

inline void BackV(std::vector<Vertex>& v, int x, int y, int z) {
    Vertex Back[] = {
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
        {x+0.5f, y-0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
        {x+0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        {x+0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        {x-0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
    };
    v.insert(v.end(), &Back[0], &Back[6]);
}

inline void FrontV(std::vector<Vertex>& v, int x, int y, int z) {
    Vertex Front[] = {
        {x-0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {x+0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
        {x+0.5f, y+0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        {x+0.5f, y+0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        {x-0.5f, y+0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        {x-0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    };
    v.insert(v.end(), &Front[0], &Front[6]);
}

inline void LeftV(std::vector<Vertex>& v, int x, int y, int z) {
    Vertex Left[] = {
        {x-0.5f, y+0.5f, z+0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {x-0.5f, y+0.5f, z-0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        {x-0.5f, y-0.5f, z-0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        {x-0.5f, y-0.5f, z-0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        {x-0.5f, y-0.5f, z+0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {x-0.5f, y+0.5f, z+0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    };
    v.insert(v.end(), &Left[0], &Left[6]);
}

inline void RightV(std::vector<Vertex>& v, int x, int y, int z) {
    Vertex Right[] = {
        {x+0.5f, y+0.5f, z+0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {x+0.5f, y+0.5f, z-0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        {x+0.5f, y-0.5f, z-0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        {x+0.5f, y-0.5f, z-0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        {x+0.5f, y-0.5f, z+0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {x+0.5f, y+0.5f, z+0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    };
    v.insert(v.end(), &Right[0], &Right[6]);
}

inline void BottomV(std::vector<Vertex>& v, int x, int y, int z) {
    Vertex Bottom[] = {
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
        {x+0.5f, y-0.5f, z-0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f},
        {x+0.5f, y-0.5f, z+0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
        {x+0.5f, y-0.5f, z+0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
        {x-0.5f, y-0.5f, z+0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
    };
    v.insert(v.end(), &Bottom[0], &Bottom[6]);
}

inline void TopV(std::vector<Vertex>& v, int x, int y, int z) {
    Vertex Top[] = {
        {x-0.5f, y+0.5f, z-0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        {x+0.5f, y+0.5f, z-0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
        {x+0.5f, y+0.5f, z+0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        {x+0.5f, y+0.5f, z+0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        {x-0.5f, y+0.5f, z+0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {x-0.5f, y+0.5f, z-0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}
    };
    v.insert(v.end(), &Top[0], &Top[6]);
}

const Vertex Cube[] = {
    // back face
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
    { 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
    {-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},

    // front face
    {-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

    // left face
    {-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},

    // right face
    {0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f},

    // bottom face
    {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f},
    { 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},

    // top face
    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f}
};

using BlockType = uint8_t;

const BlockType AIR = 0;
const BlockType DIRT = 1;
const BlockType SAND = 2;
const BlockType STONE = 3;

const unsigned int CHUNK_SIZE = 16;
const unsigned int WORLD_SIZE = 1;

const unsigned int CHUNK_LIST_SIZE = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
const unsigned int WORLD_LIST_SIZE = WORLD_SIZE * WORLD_SIZE * WORLD_SIZE;

struct ChunkData
{
    BlockType blocks[CHUNK_LIST_SIZE];
    bool updateMesh = true;
    Mesh mesh;
    int world_pos[3] = {0, 0, 0};
};

struct WorldData
{
    ChunkData chunks[WORLD_LIST_SIZE];
};

BlockType GetBlockAtWorldPos(const WorldData& world, float x, float y, float z);

bool UpdateMeshForChunkAtWorldPos(WorldData& world, float x, float y, float z);

void CreateMeshForChunk(ChunkData& chunk);

inline int Flatten(int x, int y, int z, int size)
{
    return x + y * size + z * size * size;
}

void GenerateWorld(WorldData& world);
void RenderWorld(WorldData& world);


}
}

#endif