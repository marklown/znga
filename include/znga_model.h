#ifndef ZNGA_MODEL_H
#define ZNGA_MODEL_H

#include "znga_mesh.h"
#include "linmath.h"

typedef struct znga_model_t
{
    znga_mesh_t* meshes;
    unsigned int num_meshes;
} znga_model_t;

typedef struct znga_model_instance_t
{
    znga_model_t* model;
    mat4x4 transform;
} znga_model_instance_t;

znga_model_t znga_model_create(const char* path);
void znga_model_free(znga_model_t* model);
void znga_model_draw(znga_model_t* model);

znga_model_instance_t znga_model_create_instance(znga_model_t* model, mat4x4 transform);
void znga_model_draw_instance(znga_model_instance_t* model_instance);



#endif