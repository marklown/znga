
#ifndef ZNGA_CUBE_H
#define ZNGA_CUBE_H

#include "Mesh.h"
#include "BlockInfo.h"
#include "Collision.h"
#include <cassert>
#include <map>
#include <array>
#include <queue>
#include <thread>
#include <mutex>

namespace Znga {
namespace Graphics {

using Light = unsigned short;

const int CHUNK_SIZE_X = 128;
const int CHUNK_SIZE_Y = 128;
const int CHUNK_SIZE_Z = 128;

const int WORLD_SIZE_X = 1;
const int WORLD_SIZE_Y = 1;
const int WORLD_SIZE_Z = 1;

struct Chunk
{
    Block blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    Light pointlight[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    Light sunlight[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    int world_pos[3] = {0, 0, 0};
    Mesh mesh;
    bool has_mesh = false;
};

class World
{
public:
    World();

    void   Init();
    void   Generate();
    void   Update();
    void   Render();
    void   Generate(Chunk* chunk);

    Chunk* GetChunkByPos(int x, int y, int z);
    Chunk* AddChunkByPos(int x, int y, int z);
    Chunk* GetChunkByIndex(int i, int j, int k);
    Chunk* AddChunkByIndex(int i, int j, int k);
    void   UpdateMesh(Chunk* chunk);

    Block GetBlock(int x, int y, int z);

    Light GetPointlight(int x, int y, int z);
    void  SetPointlight(int x, int y, int z, Light value);
    void  PlacePointlight(int x, int y, int z);

    Light GetSunlight(int x, int y, int z);
    void  SetSunlight(int x, int y, int z, Light value);
    void  PropogateSunlight(Chunk* chunk);
    
    void ProcessUpdateQueue();
    void ProcessGenQueue();

    bool Collides(const Znga::Physics::AABB& other);

private:

    int HashFromPos(int x, int y, int z);
    int HashFromIndex(int i, int j, int k);

    Chunk* GetChunk(int hash);
    Chunk* AddChunk(int hash);

    std::map<int, Chunk*> m_chunks;

    TextureAtlas* m_blockAtlas;
    std::map<Block, BlockInfo*> m_blockInfoMap;
    GrassBlockInfo* m_grassBlockInfo;
    DirtBlockInfo* m_dirtBlockInfo;
    SandBlockInfo* m_sandBlockInfo;
    StoneBlockInfo* m_stoneBlockInfo;

    std::queue<Chunk*> m_genQueue;
    std::queue<Chunk*> m_updateQueue;
};

struct LightNode
{
    LightNode(int xx, int yy, int zz)
        : x(xx), y(yy), z(zz) {}
    int x, y, z;
};


}
}

#endif
