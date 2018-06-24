#ifndef ZNGA_TERRAIN_H
#define ZNGA_TERRAIN_H

#include "Mesh.h"

namespace Znga {
namespace Graphics {

Mesh CreateSmoothTerrain(unsigned int size);
Mesh CreateFlatTerrain(unsigned int size);

}
}

#endif