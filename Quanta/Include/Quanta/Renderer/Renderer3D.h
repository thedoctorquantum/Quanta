#pragma once

#include "../Core/Windowing/Window.h"
#include "Mesh.h"
#include "Material.h"

namespace Quanta::Renderer3D
{
    void Initialize(const Window& window);
    void DeInitialize();

    const glm::mat4& GetView();
    void SetView(const glm::mat4&);

    void BeginPass();
    void EndPass();

    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform);
}