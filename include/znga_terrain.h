#ifndef ZNGA_TERRAIN_H
#define ZNGA_TERRAIN_H

#include "znga_mesh.h"

namespace Znga {
namespace Graphics {

Mesh* CreateSmoothTerrain(unsigned int size, const Shader& shader);
Mesh* CreateFlatTerrain(unsigned int size, const Shader& shader);

}
}

#endif