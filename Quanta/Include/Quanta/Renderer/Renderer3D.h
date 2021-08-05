#pragma once

#include <vector>

#include "../Core/Windowing/Window.h"
#include "Mesh.h"
#include "Material.h"
#include "PointLight.h"

namespace Quanta::Renderer3D
{
    void Initialize(const Window& window);
    void DeInitialize();

    const glm::mat4& GetView();
    void SetView(const glm::mat4& matrix, const glm::vec3& position);

    void BeginPass();
    void EndPass();
    
    void SetLights(const PointLight* lights, size_t count);
    
    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform);
}