
#ifndef ZNGA_CUBE_H
#define ZNGA_CUBE_H

#include <GLFW/glfw3.h>

#include "znga_mesh.h"

struct znga_cube_t
{
    znga_mesh_t mesh;
};

znga_cube_t znga_create_cube();

#endif