#include "Cube.h"

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

    // Loop over all blocks in the chunk
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {

                int index = Flatten(i, j, k, CHUNK_SIZE);

                if (AIR == chunk.blocks[index]) {
                    continue;
                }

                if (i < CHUNK_SIZE - 1) {
                    int rightIndex = Flatten(i + 1, j, k, CHUNK_SIZE);
                    if (AIR != chunk.blocks[rightIndex]) {
                        vertices.insert(vertices.end(), &Right[0], &Right[6]);
                    }
                } 

                if (i > 0) {
                    int leftIndex = Flatten(i - 1, j, k, CHUNK_SIZE);
                    if (AIR != chunk.blocks[leftIndex]) {
                        vertices.insert(vertices.end(), &Left[0], &Left[6]);
                    }
                } 

                if (j < CHUNK_SIZE - 1) {
                    int topIndex = Flatten(i, j + 1, k, CHUNK_SIZE);
                    if (AIR != chunk.blocks[topIndex]) {
                        vertices.insert(vertices.end(), &Top[0], &Top[6]);
                    }
                } 

                if (j > 0) {
                    int botIndex = Flatten(i, j - 1, k, CHUNK_SIZE);
                    if (AIR != chunk.blocks[botIndex]) {
                        vertices.insert(vertices.end(), &Bottom[0], &Bottom[6]);
                    }
                } 

                if (k < CHUNK_SIZE - 1) {
                    int frontIndex = Flatten(i, j, k + 1, CHUNK_SIZE);
                    if (AIR != chunk.blocks[frontIndex]) {
                        vertices.insert(vertices.end(), &Front[0], &Front[6]);
                    }
                } 

                if (k > 0) {
                    int backIndex = Flatten(i, j, k - 1, CHUNK_SIZE);
                    if (AIR != chunk.blocks[backIndex]) {
                        vertices.insert(vertices.end(), &Back[0], &Back[6]);
                    }
                } 
            }
        }
    }

    chunk.mesh = CreateMesh(vertices);
}

}
}