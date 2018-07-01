
#ifndef ZNGA_CUBE_H
#define ZNGA_CUBE_H

#include "Mesh.h"
#include <cassert>
#include <map>

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

using Block = unsigned short;
using Light = unsigned short;

const Block AIR = 0;
const Block DIRT = 1;
const Block SAND = 2;
const Block STONE = 3;

const int CHUNK_SIZE = 16;
const int WORLD_SIZE_X = 2;
const int WORLD_SIZE_Y = 1;
const int WORLD_SIZE_Z = 1;

struct Chunk
{
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Light pointlight[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Light sunlight[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    int world_pos[3] = {0, 0, 0};
    Mesh mesh;
};

class World
{
public:

    struct Key
    {
        Key(int ii, int jj, int kk) : i(ii), j(jj), k(kk) {}
        int i = 0; int j = 0; int k = 0;
    };

    struct KeyCompare 
    {
        bool operator() (const Key& lhs, const Key& rhs) const {
            return lhs.i < rhs.i && lhs.j < rhs.j && lhs.k < rhs.k;
        }
    };

    void   Generate();
    void   Update();
    void   Render();

    Chunk* GetChunk(const Key& key);
    Chunk* GetChunk(int wx, int wy, int wz);
    Chunk* AddChunk(const Key& key);
    Chunk* AddChunk(int wx, int wy, int wz);
    void   UpdateMesh(Chunk* chunk);

    Block GetBlock(int wx, int wy, int wz);

    Light GetPointlight(int wx, int wy, int wz);
    void  SetPointlight(int wx, int wy, int wz, Light value);
    void  PlacePointlight(int wx, int wy, int wz);
    
private:

    std::map<Key, Chunk*, KeyCompare> chunks;

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
