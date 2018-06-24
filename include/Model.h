#ifndef ZNGA_MODEL_H
#define ZNGA_MODEL_H

#include "Mesh.h"
#include "linmath.h"

#include <string>

namespace Znga {
namespace Graphics {

class Model
{
public:
    Model() = delete;
    Model(const std::string& path);
    virtual ~Model();

    void Render() const;
    unsigned int GetNumberOfMeshes() const { return m_meshes.size(); }

private:
    std::vector<Mesh> m_meshes;
};

class ModelInstance
{
public:
    ModelInstance() = default;
    virtual ~ModelInstance() = default;
    ModelInstance(const Model* model, mat4x4 transform);

    void Render() const;

private:
    const Model* m_model = nullptr;
    mat4x4 m_transform;
    GLuint m_uniformTransform;
};

}
}

#endif