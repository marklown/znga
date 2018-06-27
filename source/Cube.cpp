#include "Cube.h"
#include <random>
#include <queue>

namespace Znga {
namespace Graphics {


void CreateMeshForChunk(Chunk& chunk)
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
}

void GenerateWorld(World& world)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, 3);

    for (unsigned int i = 0; i < WORLD_SIZE; i++) {
        for (unsigned int j = 0; j < WORLD_SIZE; j++) {
            for (unsigned int k = 0; k < WORLD_SIZE; k++) {
                Chunk& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                memset(chunk.sun_light, 0, sizeof(chunk.sun_light));
                memset(chunk.torch_light, 0, sizeof(chunk.torch_light));
                chunk.world_pos[0] = i * CHUNK_SIZE;
                chunk.world_pos[1] = j * CHUNK_SIZE;
                chunk.world_pos[2] = k * CHUNK_SIZE;
                for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
                    for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
                        for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
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

void RenderWorld(World& world)
{
    for (unsigned int i = 0; i < WORLD_SIZE; i++) {
        for (unsigned int j = 0; j < WORLD_SIZE; j++) {
            for (unsigned int k = 0; k < WORLD_SIZE; k++) {
                const Chunk& chunk = world.chunks[Flatten(i, j, k, WORLD_SIZE)];
                RenderMesh(chunk.mesh);
            }
        }
    }
}

void PlaceTorch(World& world, unsigned int x, unsigned int y, unsigned int z)
{
    std::queue<LightNode> queue;

    SetTorchlight(world, x, y, z, 14);

    unsigned int index = Flatten(x, y, z, CHUNK_SIZE);
    queue.emplace(index, world.chunks[index]);

    while (!queue.empty()) {
        LightNode& node = queue.front();
        index = node.index;
        Chunk& chunk = node.chunk;
        queue.pop();
        unsigned int z = index % CHUNK_SIZE;
        unsigned int y = index / (CHUNK_SIZE * CHUNK_SIZE);
        unsigned int x = (index % (CHUNK_SIZE * CHUNK_SIZE)) / CHUNK_SIZE;

        Light light_level = GetTorchlight(world, x, y, z);

        if (x > 0 && x < WORLD_SIZE * CHUNK_SIZE - 1) {
            // Left neighbor
            if ((GetBlock(world, x - 1, y, z) < DIRT) &&
                (GetTorchlight(world, x - 1, y, z) + 2 <= light_level)) {
                    SetTorchlight(world, x - 1, y, z, light_level - 1);
                    index = Flatten(x - 1, y, z, CHUNK_SIZE);
                    queue.emplace(index, GetChunk(world, x - 1, y, z));
            }

            // Right neighbor
            if ((GetBlock(world, x + 1, y, z) < DIRT) &&
                (GetTorchlight(world, x + 1, y, z) + 2 <= light_level)) {
                    SetTorchlight(world, x + 1, y, z, light_level - 1);
                    index = Flatten(x + 1, y, z, CHUNK_SIZE);
                    queue.emplace(index, GetChunk(world, x + 1, y, z));
            }
        }

        if (y > 0 && y < WORLD_SIZE * CHUNK_SIZE - 1) {
            // Bottom neighbor
            if ((GetBlock(world, x, y - 1, z) < DIRT) &&
                (GetTorchlight(world, x, y - 1, z) + 2 <= light_level)) {
                    SetTorchlight(world, x, y - 1, z, light_level - 1);
                    index = Flatten(x, y - 1, z, CHUNK_SIZE);
                    queue.emplace(index, GetChunk(world, x, y - 1, z));
            }

            // Top neighbor
            if ((GetBlock(world, x, y + 1, z) < DIRT) &&
                (GetTorchlight(world, x, y + 1, z) + 2 <= light_level)) {
                    SetTorchlight(world, x, y + 1, z, light_level - 1);
                    index = Flatten(x, y + 1, z, CHUNK_SIZE);
                    queue.emplace(index, GetChunk(world, x, y + 1, z));
            }

        }

        if (z > 0 && z < WORLD_SIZE * CHUNK_SIZE - 1) {
            // Back neighbor
            if ((GetBlock(world, x, y, z - 1) < DIRT) &&
                (GetTorchlight(world, x, y, z - 1) + 2 <= light_level)) {
                    SetTorchlight(world, x, y, z - 1, light_level - 1);
                    index = Flatten(x, y, z - 1, CHUNK_SIZE);
                    queue.emplace(index, GetChunk(world, x, y, z - 1));
            }

            // Front neighbor
            if ((GetBlock(world, x, y, z + 1) < DIRT) &&
                (GetTorchlight(world, x, y, z + 1) + 2 <= light_level)) {
                    SetTorchlight(world, x, y, z + 1, light_level - 1);
                    index = Flatten(x, y, z + 1, CHUNK_SIZE);
                    queue.emplace(index, GetChunk(world, x, y, z + 1));
            }

        }
    }


}


}
}