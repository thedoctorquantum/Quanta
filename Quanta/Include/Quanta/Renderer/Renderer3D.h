#pragma once

#include <vector>

#include "../Core/Windowing/Window.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "PointLight.h"
#include "DirectionalLight.h"

namespace Quanta::Renderer3D
{
    struct View final
    {
    public:
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        
        float fieldOfView = 45.0f;
        float near = 0.1f;
        float far = 1000.0f;
    };
    
    void Create(const Window& window);
    void Destroy();

    void BeginPass(const View& view);
    void EndPass();
    
    void SetDirectionalLight(const DirectionalLight& light);

    void SetPointLights(const PointLight* lights, size_t count);
    
    void SetEnvironmentSampler(const std::shared_ptr<Sampler>&);

    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform);
    void DrawModel(const Model& model, const glm::mat4& transform);
}