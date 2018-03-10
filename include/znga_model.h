#ifndef ZNGA_MODEL_H
#define ZNGA_MODEL_H

#include "znga_mesh.h"

typedef struct znga_model_t
{
    znga_mesh_t* meshes;
    unsigned int num_meshes;
} znga_model_t;

znga_model_t znga_create_model(const char* path);
void znga_free_model(znga_model_t* model);



#endif