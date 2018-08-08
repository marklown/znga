
#ifndef ZNGA_BLOCKINFO_H
#define ZNGA_BLOCKINFO_H

#include "Mesh.h"
#include <cassert>
#include <map>
#include <array>

namespace Znga {
namespace Graphics {

using Block = unsigned short;
const Block AIR = 0;
const Block DIRT = 1;
const Block GRASS = 2;
const Block SAND = 3;
const Block STONE = 4;

const Block ERR = 254;


enum Face 
{
    NONE = -1,
    FRONT,
    RIGHT,
    BACK,
    LEFT,
    TOP,
    BOTTOM,
};

struct FaceInfo 
{
    FaceInfo(Face f, GLfloat pl, GLfloat sl, GLfloat a)
        : face(f), point_light(pl), sun_light(sl), ao(a) {}
    Face face = NONE;
    GLfloat point_light = 0;
    GLfloat sun_light = 0;
    GLfloat ao = 0;
};

struct BlockInfo
{
    Block type;
    GLfloat front_uv[12];
    GLfloat right_uv[12];
    GLfloat back_uv[12];
    GLfloat left_uv[12];
    GLfloat top_uv[12];
    GLfloat bottom_uv[12];
};

struct DirtBlockInfo : public BlockInfo 
{
    DirtBlockInfo(const TextureAtlas& atlas)
    {
        type = DIRT;
        TextureInfo info = atlas.GetTextureInfo("GrassDirt.png");
        front_uv[0] = info.u[0]; front_uv[1] = info.v[0];
        front_uv[2] = info.u[1]; front_uv[3] = info.v[1];
        front_uv[4] = info.u[2]; front_uv[5] = info.v[2];
        front_uv[6] = info.u[2]; front_uv[7] = info.v[2];
        front_uv[8] = info.u[3]; front_uv[9] = info.v[3];
        front_uv[10] = info.u[0]; front_uv[11] = info.v[0];
        memcpy(right_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(back_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(left_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(top_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(bottom_uv, front_uv, 12 * sizeof(GLfloat));
    }
};

struct GrassBlockInfo : public BlockInfo 
{
    GrassBlockInfo(const TextureAtlas& atlas)
    {
        type = GRASS;
        TextureInfo info = atlas.GetTextureInfo("GrassSide.png");
        front_uv[0] = info.u[0]; front_uv[1] = info.v[0];
        front_uv[2] = info.u[1]; front_uv[3] = info.v[1];
        front_uv[4] = info.u[2]; front_uv[5] = info.v[2];
        front_uv[6] = info.u[2]; front_uv[7] = info.v[2];
        front_uv[8] = info.u[3]; front_uv[9] = info.v[3];
        front_uv[10] = info.u[0]; front_uv[11] = info.v[0];
        memcpy(right_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(back_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(left_uv, front_uv, 12 * sizeof(GLfloat));

        info = atlas.GetTextureInfo("GrassTop.png");
        top_uv[0] = info.u[0]; top_uv[1] = info.v[0];
        top_uv[2] = info.u[1]; top_uv[3] = info.v[1];
        top_uv[4] = info.u[2]; top_uv[5] = info.v[2];
        top_uv[6] = info.u[2]; top_uv[7] = info.v[2];
        top_uv[8] = info.u[3]; top_uv[9] = info.v[3];
        top_uv[10] = info.u[0]; top_uv[11] = info.v[0];

        info = atlas.GetTextureInfo("GrassDirt.png");
        bottom_uv[0] = info.u[0]; bottom_uv[1] = info.v[0];
        bottom_uv[2] = info.u[1]; bottom_uv[3] = info.v[1];
        bottom_uv[4] = info.u[2]; bottom_uv[5] = info.v[2];
        bottom_uv[6] = info.u[2]; bottom_uv[7] = info.v[2];
        bottom_uv[8] = info.u[3]; bottom_uv[9] = info.v[3];
        bottom_uv[10] = info.u[0]; bottom_uv[11] = info.v[0];
    }
};

struct SandBlockInfo : public BlockInfo 
{
    SandBlockInfo(const TextureAtlas& atlas)
    {
        type = DIRT;
        TextureInfo info = atlas.GetTextureInfo("Quicksoil.png");
        front_uv[0] = info.u[0]; front_uv[1] = info.v[0];
        front_uv[2] = info.u[1]; front_uv[3] = info.v[1];
        front_uv[4] = info.u[2]; front_uv[5] = info.v[2];
        front_uv[6] = info.u[2]; front_uv[7] = info.v[2];
        front_uv[8] = info.u[3]; front_uv[9] = info.v[3];
        front_uv[10] = info.u[0]; front_uv[11] = info.v[0];
        memcpy(right_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(back_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(left_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(top_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(bottom_uv, front_uv, 12 * sizeof(GLfloat));
    }
};

struct StoneBlockInfo : public BlockInfo 
{
    StoneBlockInfo(const TextureAtlas& atlas)
    {
        type = STONE;
        TextureInfo info = atlas.GetTextureInfo("stone.png");
        front_uv[0] = info.u[0]; front_uv[1] = info.v[0];
        front_uv[2] = info.u[1]; front_uv[3] = info.v[1];
        front_uv[4] = info.u[2]; front_uv[5] = info.v[2];
        front_uv[6] = info.u[2]; front_uv[7] = info.v[2];
        front_uv[8] = info.u[3]; front_uv[9] = info.v[3];
        front_uv[10] = info.u[0]; front_uv[11] = info.v[0];
        memcpy(right_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(back_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(left_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(top_uv, front_uv, 12 * sizeof(GLfloat));
        memcpy(bottom_uv, front_uv, 12 * sizeof(GLfloat));
    }
};

struct MeshFactory
{
    static std::vector<Vertex> CreateBlockMesh(int x, int y, int z,
                                               const BlockInfo& blockInfo,
                                               const std::vector<FaceInfo>& faceInfo)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(6 * faceInfo.size());

        for (int i=0;i<faceInfo.size();i++) {
            const FaceInfo& fi = faceInfo[i];
            switch (fi.face) {
                case FRONT: {
                    Vertex v0 = {x-.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.front_uv[0], blockInfo.front_uv[1], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v1 = {x+.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.front_uv[2], blockInfo.front_uv[3], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v2 = {x+.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.front_uv[4], blockInfo.front_uv[5], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v3 = {x+.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.front_uv[6], blockInfo.front_uv[7], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v4 = {x-.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.front_uv[8], blockInfo.front_uv[9], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v5 = {x-.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.front_uv[10], blockInfo.front_uv[11], fi.point_light, 0, fi.sun_light, 0};
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    break;
                }
                case RIGHT: {
                    Vertex v0 = {x+.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.right_uv[0], blockInfo.right_uv[1], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v1 = {x+.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.right_uv[2], blockInfo.right_uv[3], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v2 = {x+.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.right_uv[4], blockInfo.right_uv[5], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v3 = {x+.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.right_uv[6], blockInfo.right_uv[7], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v4 = {x+.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.right_uv[8], blockInfo.right_uv[9], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v5 = {x+.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.right_uv[10], blockInfo.right_uv[11], fi.point_light, 0, fi.sun_light, 0};
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    break;
                }
                case BACK: {
                    Vertex v0 = {x+.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.back_uv[0], blockInfo.back_uv[1], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v1 = {x-.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.back_uv[2], blockInfo.back_uv[3], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v2 = {x-.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.back_uv[4], blockInfo.back_uv[5], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v3 = {x-.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.back_uv[6], blockInfo.back_uv[7], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v4 = {x+.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.back_uv[8], blockInfo.back_uv[9], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v5 = {x+.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.back_uv[10], blockInfo.back_uv[11], fi.point_light, 0, fi.sun_light, 0};
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    break;
                }
                case LEFT: {
                    Vertex v0 = {x-.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.left_uv[0], blockInfo.left_uv[1], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v1 = {x-.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.left_uv[2], blockInfo.left_uv[3], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v2 = {x-.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.left_uv[4], blockInfo.left_uv[5], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v3 = {x-.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.left_uv[6], blockInfo.left_uv[7], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v4 = {x-.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.left_uv[8], blockInfo.left_uv[9], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v5 = {x-.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.left_uv[10], blockInfo.left_uv[11], fi.point_light, 0, fi.sun_light, 0};
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    break;
                }
                case TOP: {
                    Vertex v0 = {x-.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.top_uv[0], blockInfo.top_uv[1], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v1 = {x+.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.top_uv[2], blockInfo.top_uv[3], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v2 = {x+.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.top_uv[4], blockInfo.top_uv[5], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v3 = {x+.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.top_uv[6], blockInfo.top_uv[7], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v4 = {x-.5f, y+.5f, z-.5f, 0, 0, 1, blockInfo.top_uv[8], blockInfo.top_uv[9], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v5 = {x-.5f, y+.5f, z+.5f, 0, 0, 1, blockInfo.top_uv[10], blockInfo.top_uv[11], fi.point_light, 0, fi.sun_light, 0};
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    break;
                }
                case BOTTOM: {
                    Vertex v0 = {x-.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.bottom_uv[0], blockInfo.bottom_uv[1], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v1 = {x+.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.bottom_uv[2], blockInfo.bottom_uv[3], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v2 = {x+.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.bottom_uv[4], blockInfo.bottom_uv[5], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v3 = {x+.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.bottom_uv[6], blockInfo.bottom_uv[7], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v4 = {x-.5f, y-.5f, z+.5f, 0, 0, 1, blockInfo.bottom_uv[8], blockInfo.bottom_uv[9], fi.point_light, 0, fi.sun_light, 0};
                    Vertex v5 = {x-.5f, y-.5f, z-.5f, 0, 0, 1, blockInfo.bottom_uv[10], blockInfo.bottom_uv[11], fi.point_light, 0, fi.sun_light, 0};
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    break;
                }

            }
        }
        return vertices;
    }
};

}
}

#endif