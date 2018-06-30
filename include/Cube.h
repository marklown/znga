
#ifndef ZNGA_CUBE_H
#define ZNGA_CUBE_H

#include "Mesh.h"
#include <cassert>

namespace Znga {
namespace Graphics {

inline void BackV(std::vector<Vertex>& v, int x, int y, int z, vec4 light) {
    Vertex Back[] = {
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
    };
    v.insert(v.end(), &Back[0], &Back[6]);
}

inline void FrontV(std::vector<Vertex>& v, int x, int y, int z, vec4 light) {
    Vertex Front[] = {
        {x-0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y+0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
    };
    v.insert(v.end(), &Front[0], &Front[6]);
}

inline void LeftV(std::vector<Vertex>& v, int x, int y, int z, vec4 light) {
    Vertex Left[] = {
        {x-0.5f, y+0.5f, z+0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y+0.5f, z-0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z-0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z-0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z+0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y+0.5f, z+0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
    };
    v.insert(v.end(), &Left[0], &Left[6]);
}

inline void RightV(std::vector<Vertex>& v, int x, int y, int z, vec4 light) {
    Vertex Right[] = {
        {x+0.5f, y+0.5f, z+0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z-0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z-0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z-0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z+0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z+0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
    };
    v.insert(v.end(), &Right[0], &Right[6]);
}

inline void BottomV(std::vector<Vertex>& v, int x, int y, int z, vec4 light) {
    Vertex Bottom[] = {
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z-0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z+0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y-0.5f, z+0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z+0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y-0.5f, z-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
    };
    v.insert(v.end(), &Bottom[0], &Bottom[6]);
}

inline void TopV(std::vector<Vertex>& v, int x, int y, int z, vec4 light) {
    Vertex Top[] = {
        {x-0.5f, y+0.5f, z-0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z-0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z+0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x+0.5f, y+0.5f, z+0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y+0.5f, z+0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, light[0], light[1], light[2], light[3]},
        {x-0.5f, y+0.5f, z-0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, light[0], light[1], light[2], light[3]}
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

using BlockType = unsigned short;
using Light = unsigned short;

const BlockType AIR = 0;
const BlockType DIRT = 1;
const BlockType SAND = 2;
const BlockType STONE = 3;

const unsigned int CHUNK_SIZE = 32;
const unsigned int WORLD_SIZE = 1;

const unsigned int CHUNK_LIST_SIZE = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
const unsigned int WORLD_LIST_SIZE = WORLD_SIZE * WORLD_SIZE * WORLD_SIZE;

struct Chunk
{
    BlockType blocks[CHUNK_LIST_SIZE];
    Light torch_light[CHUNK_LIST_SIZE];
    Light sun_light[CHUNK_LIST_SIZE];
    int world_pos[3] = {0, 0, 0};
    Mesh mesh;
    bool needs_update = true;
};

struct World
{
    Chunk chunks[WORLD_LIST_SIZE];
};

struct LightNode
{
    LightNode(unsigned int x, unsigned int y, unsigned int z, Chunk& chnk)
        : x(x), y(y), z(z), chunk(chnk) {}
    unsigned int x, y, z;
    Chunk& chunk;
};


void GenerateWorld(World& world);

void UpdateWorld(World& world);

void RenderWorld(World& world);

void CreateMeshForChunk(World& world, Chunk& chunk);

void PlaceTorch(World& world, unsigned int wx, unsigned int wy, unsigned int wz);

inline unsigned int Flatten(unsigned int x, unsigned int y, unsigned int z, unsigned int size)
{
    return x + (y * size) + (z * size * size);
}

inline unsigned int WorldPosToChunkIndex(unsigned int wx, unsigned int wy, unsigned int wz)
{
    unsigned int x = wx / CHUNK_SIZE;
    unsigned int y = wy / CHUNK_SIZE;
    unsigned int z = wz / CHUNK_SIZE;
    unsigned int index = Flatten(x, y, z, WORLD_SIZE);
    assert(index < WORLD_LIST_SIZE);
    return index;
}

inline unsigned int WorldPosToBlockIndex(unsigned int wx, unsigned int wy, unsigned int wz)
{
    unsigned int x = wx % CHUNK_SIZE;
    unsigned int y = wy % CHUNK_SIZE;
    unsigned int z = wz % CHUNK_SIZE;
    unsigned int index = Flatten(x, y, z, CHUNK_SIZE);
    assert(index < CHUNK_LIST_SIZE);
    return index;
}

inline BlockType GetBlock(World& world, unsigned int x, unsigned int y, unsigned int z)
{
    unsigned int chunk_index = WorldPosToChunkIndex(x, y, z);
    unsigned int block_index = WorldPosToBlockIndex(x, y, z);
    Chunk& chunk = world.chunks[chunk_index];
    return chunk.blocks[block_index];
}

inline Chunk& GetChunk(World& world, unsigned int x, unsigned int y, unsigned int z)
{
    unsigned int chunk_index = WorldPosToChunkIndex(x, y, z);
    unsigned int block_index = WorldPosToBlockIndex(x, y, z);
    Chunk& chunk = world.chunks[chunk_index];
    return chunk;
}

inline void SetSunlight(World& world, unsigned int x, unsigned int y, unsigned int z, Light val)
{
    unsigned int chunk_index = WorldPosToChunkIndex(x, y, z);
    unsigned int block_index = WorldPosToBlockIndex(x, y, z);
    Chunk& chunk = world.chunks[chunk_index];
    chunk.sun_light[block_index] = val;
}

inline Light GetSunlight(World& world, unsigned int x, unsigned int y, unsigned int z)
{
    unsigned int chunk_index = WorldPosToChunkIndex(x, y, z);
    unsigned int block_index = WorldPosToBlockIndex(x, y, z);
    Chunk& chunk = world.chunks[chunk_index];
    return chunk.sun_light[block_index];
}

inline void SetTorchlight(World& world, unsigned int x, unsigned int y, unsigned int z, Light val)
{
    unsigned int chunk_index = WorldPosToChunkIndex(x, y, z);
    unsigned int block_index = WorldPosToBlockIndex(x, y, z);
    Chunk& chunk = world.chunks[chunk_index];
    chunk.torch_light[block_index] = val;
}

inline Light GetTorchlight(World& world, unsigned int x, unsigned int y, unsigned int z)
{
    unsigned int chunk_index = WorldPosToChunkIndex(x, y, z);
    unsigned int block_index = WorldPosToBlockIndex(x, y, z);
    Chunk& chunk = world.chunks[chunk_index];
    return chunk.torch_light[block_index];
}


}
}

#endif
