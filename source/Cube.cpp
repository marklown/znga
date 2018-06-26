#include "Cube.h"
#include <random>

namespace Znga {
namespace Graphics {

int WorldPosToChunkIndex(int wX, int wY, int wZ)
{
    int x = wX / CHUNK_SIZE;
    int y = wY / CHUNK_SIZE;
    int z = wZ / CHUNK_SIZE;

    int index = x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE;

    if (index > WORLD_LIST_SIZE) {
        return -1;
    }

    return index;
}

int WorldPosToBlockIndex(int wX, int wY, int wZ)
{
    int x = wX % CHUNK_SIZE;
    int y = wY % CHUNK_SIZE;
    int z = wZ % CHUNK_SIZE;

    int index = x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE; 

    if (index > CHUNK_LIST_SIZE) {
        return -1;
    }

    return index;
}

BlockType GetBlockAtWorldPos(const WorldData& world, float fx, float fy, float fz)
{
    int x = (int)fx;
    int y = (int)fy;
    int z = (int)fz;

    // Find the chunk at this world pos
    int chunkIndex = WorldPosToChunkIndex(x, y, z);
    if (chunkIndex < 0) {
        return AIR;
    }

    // Look up the BlockType
    int blockIndex = WorldPosToBlockIndex(x, y, z);
    if (blockIndex < 0) {
        return AIR;
    }
    return world.chunks[chunkIndex].blocks[blockIndex];
}

bool UpdateMeshForChunkAtWorldPos(WorldData& world, float fx, float fy, float fz)
{
    int x = (int)fx;
    int y = (int)fy;
    int z = (int)fz;

    int chunkIndex = WorldPosToChunkIndex(x, y, z);
    if (chunkIndex < 0) {
        return false;
    }

    ChunkData& chunk = world.chunks[chunkIndex];

    if (chunk.updateMesh) {
        CreateMeshForChunk(chunk);
    }

    return true;
}

void CreateMeshForChunk(ChunkData& chunk)
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

                if (i < CHUNK_SIZE - 1) {
                    int rightIndex = Flatten(i + 1, j, k, CHUNK_SIZE);
                    if (AIR == chunk.blocks[rightIndex]) {
                        RightV(vertices, x, y, z);
                    }
                } else {
                    RightV(vertices, x, y, z);
                }

                if (i > 0) {
                    int leftIndex = Flatten(i - 1, j, k, CHUNK_SIZE);
                    if (AIR == chunk.blocks[leftIndex]) {
                        LeftV(vertices, x, y, z);
                    }
                } else {
                    LeftV(vertices, x, y, z);
                }

                if (j < CHUNK_SIZE - 1) {
                    int topIndex = Flatten(i, j + 1, k, CHUNK_SIZE);
                    if (AIR == chunk.blocks[topIndex]) {
                        TopV(vertices, x, y, z);
                    }
                } else {
                    TopV(vertices, x, y, z);
                }

                if (j > 0) {
                    int botIndex = Flatten(i, j - 1, k, CHUNK_SIZE);
                    if (AIR == chunk.blocks[botIndex]) {
                        BottomV(vertices, x, y, z);
                    }
                } else {
                    BottomV(vertices, x, y, z);
                }

                if (k < CHUNK_SIZE - 1) {
                    int frontIndex = Flatten(i, j, k + 1, CHUNK_SIZE);
                    if (AIR == chunk.blocks[frontIndex]) {
                        FrontV(vertices, x, y, z);
                    }
                } else {
                    FrontV(vertices, x, y, z);
                }

                if (k > 0) {
                    int backIndex = Flatten(i, j, k - 1, CHUNK_SIZE);
                    if (AIR == chunk.blocks[backIndex]) {
                        BackV(vertices, x, y, z);
                    }
                } else {
                    BackV(vertices, x, y, z);
                }
            }
        }
    }

    chunk.mesh = CreateMesh(vertices);
    chunk.updateMesh = false;
}

void GenerateWorld(WorldData& world)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, 3);

    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
            for (int k = 0; k < WORLD_SIZE; k++) {
                ChunkData& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                chunk.world_pos[0] = i * CHUNK_SIZE;
                chunk.world_pos[1] = j * CHUNK_SIZE;
                chunk.world_pos[2] = k * CHUNK_SIZE;
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    for (int y = 0; y < CHUNK_SIZE; y++) {
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            int type = dist(generator);
                            chunk.blocks[Flatten(x, y, z, CHUNK_SIZE)] = (BlockType)type;
                        }
                    }
                }
                CreateMeshForChunk(chunk);
            }
        }
    }
}

void RenderWorld(WorldData& world)
{
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
            for (int k = 0; k < WORLD_SIZE; k++) {
                const ChunkData& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                RenderMesh(chunk.mesh);
            }
        }
    }
}

}
}