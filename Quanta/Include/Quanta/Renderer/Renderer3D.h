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
    void Initialize(const Window& window);
    void DeInitialize();

    const glm::mat4& GetView();
    void SetView(const glm::mat4& matrix, const glm::vec3& position);

    void BeginPass();
    void EndPass();
    
    void SetDirectionalLight(const DirectionalLight& light);

    void SetPointLights(const PointLight* lights, size_t count);
    
    void SetEnvironmentSampler(const std::shared_ptr<Sampler>&);

    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform);
    void DrawModel(const Model& model, const glm::mat4& transform);
}