
#ifndef ZNGA_CUBE_H
#define ZNGA_CUBE_H

#include "Mesh.h"
#include "BlockInfo.h"
#include <cassert>
#include <map>
#include <array>

namespace Znga {
namespace Graphics {

using Light = unsigned short;

const int CHUNK_SIZE_X = 32;
const int CHUNK_SIZE_Y = 32;
const int CHUNK_SIZE_Z = 32;

const int WORLD_SIZE_X = 8;
const int WORLD_SIZE_Y = 1;
const int WORLD_SIZE_Z = 8;

struct Chunk
{
    Block blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    Light pointlight[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    Light sunlight[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    int world_pos[3] = {0, 0, 0};
    Mesh mesh;
};

class World
{
public:
    World();

    void   Init();
    void   Generate();
    void   Update();
    void   Render();

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
