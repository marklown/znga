#include "znga_model.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <stdio.h>
#include <stdlib.h>

namespace Znga {
namespace Graphics {

Model::Model(const std::string& path)
{
    printf("Loading model: %s\n", path.c_str());

    const aiScene* scene = aiImportFile(path.c_str(),
                                        aiProcess_CalcTangentSpace |
                                        aiProcess_Triangulate |
                                        aiProcess_FlipUVs);
    if (!scene) {
        printf("Failed to load model: %s\n", path.c_str());
        return;
    }

    aiMesh* ai_mesh = scene->mMeshes[0];
    if (!ai_mesh) {
        printf("Model has no meshes: %s\n", path.c_str());
        return;
    }

    unsigned int num_vertices = ai_mesh->mNumVertices;
    unsigned int num_indices = ai_mesh->mNumFaces * 3;

    printf("num_vertices: %u\n", num_vertices);
    printf("num_indices: %u\n", num_indices);

    std::vector<Vertex> vertices(num_vertices);
    std::vector<GLuint> indices(num_indices);

    for (unsigned int i = 0; i < num_vertices; i++)
    {
        aiVector3D pos = ai_mesh->mVertices[i];
        memcpy(vertices[i].position, &pos, sizeof(GLfloat) * 3);

        aiVector3D normal = ai_mesh->mNormals[i];
        memcpy(vertices[i].normal, &normal, sizeof(GLfloat) * 3);

        aiVector3D uv = ai_mesh->mTextureCoords[0][i];
        memcpy(vertices[i].tex_coord, &uv, sizeof(GLfloat) * 2);
    }

    unsigned int index = 0;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        aiFace face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < 3; j++)
        {
            indices[index++] = face.mIndices[j];
        }
    }

    Material material;
    Mesh* mesh = new Mesh(vertices, indices, material);

    m_meshes.push_back(mesh);

    printf("num_meshes: %u\n", m_meshes.size());
    printf("Loaded model: %s\n", path.c_str());

    aiReleaseImport(scene);
}

Model::~Model()
{
    for (Mesh* m : m_meshes) {
        delete m;
    }
    m_meshes.clear();
}

void Model::Render() const 
{
    for (Mesh* m : m_meshes) {
        if (m) {
            m->Render();
        }
    }
}


ModelInstance::ModelInstance(const Model* model, mat4x4 transform)
    : m_model(model)
{
    mat4x4_dup(m_transform, transform);
}

void ModelInstance::Render() const
{
    if (m_model) {
        // TODO, set transform uniform of meshes correctly
        m_model->Render();
    }
}

}
}