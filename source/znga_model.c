#include "znga_model.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <stdio.h>
#include <stdlib.h>


znga_model_t znga_model_create(const char* path)
{
    printf("Loading model: %s\n", path);

    znga_model_t model;
    model.meshes = NULL;

    const struct aiScene* scene = aiImportFile(path,
                                               aiProcess_CalcTangentSpace |
                                               aiProcess_Triangulate |
                                               aiProcess_FlipUVs);
    if (!scene)
    {
        printf("Failed to load model: %s\n", path);
        return model;
    }

    struct aiMesh* ai_mesh = scene->mMeshes[0];
    if (!ai_mesh)
    {
        printf("Model has no meshes: %s\n", path);
        return model;
    }

    unsigned int num_vertices = ai_mesh->mNumVertices;
    unsigned int num_indices = ai_mesh->mNumFaces * 3;

    printf("num_vertices: %u\n", num_vertices);
    printf("num_indices: %u\n", num_indices);

    znga_vertex_t* vertices = malloc(sizeof(znga_vertex_t) * num_vertices);
    GLuint* indices = malloc(sizeof(GLuint) * num_indices);

    for (unsigned int i = 0; i < num_vertices; i++)
    {
        struct aiVector3D pos = ai_mesh->mVertices[i];
        memcpy(vertices[i].position, &pos, sizeof(GLfloat) * 3);

        struct aiVector3D normal = ai_mesh->mNormals[i];
        memcpy(vertices[i].normal, &normal, sizeof(GLfloat) * 3);

        struct aiVector3D uv = ai_mesh->mTextureCoords[0][i];
        memcpy(vertices[i].tex_coord, &uv, sizeof(GLfloat) * 2);
    }

    unsigned int index = 0;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        struct aiFace face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < 3; j++)
        {
            indices[index++] = face.mIndices[j];
        }
    }

    znga_material_t material;
    znga_mesh_t mesh = znga_mesh_create(vertices, num_vertices, indices, num_indices, material);
    free(vertices);
    free(indices);
    
    model.num_meshes = 1;
    model.meshes = malloc(sizeof(znga_mesh_t) * model.num_meshes);
    model.meshes[0] = mesh;

    printf("num_meshes: %u\n", model.num_meshes);
    printf("Loaded model: %s\n", path);

    aiReleaseImport(scene);
    return model;
}

void znga_model_free(znga_model_t* model)
{
    if (!model)
    {
        return;
    }
    if (!model->meshes)
    {
        return;
    }
    free(model->meshes);
}

void znga_model_draw(znga_model_t* model)
{
    if (!model)
    {
        return;
    }
    for (unsigned int i = 0; i < model->num_meshes; i++)
    {
        znga_mesh_draw(&(model->meshes[i]));
    }
}

znga_model_instance_t znga_model_create_instance(znga_model_t* model, mat4x4 transform)
{
    znga_model_instance_t model_instance;
    model_instance.model = model;
    mat4x4_dup(model_instance.transform, transform);
    return model_instance;
}

void znga_model_draw_instance(znga_model_instance_t* model_instance)
{
    if (!model_instance || !model_instance->model)
    {
        return;
    }

    znga_model_t* model = model_instance->model;

    znga_shader_set_uniform_mat4(model->meshes[0].material.shader.loc_u_model,
                                 (GLfloat*)model_instance->transform);

    znga_model_draw(model);
}