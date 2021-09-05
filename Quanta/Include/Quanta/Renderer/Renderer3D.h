#pragma once

#include <vector>

#include "../Core/Windowing/Window.h"
#include "../Graphics/Texture/FrameBuffer.h"
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
        glm::mat4 matrix = glm::mat4(1.0f); 
        
        float width = 0;
        float height = 0;
        float far = 1000.0f;
        float near = 0.1f;
        float fieldOfView = 45.0f;
    };
    
    void Create(const Window& window, const std::shared_ptr<FrameBuffer>& frameBuffer);
    void Destroy();

    void BeginPass(const View& view);
    void EndPass();

    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();

    void EnableWireframe(bool enable);

    void SetViewProjection(const glm::mat4&);
    
    void SetDirectionalLight(const DirectionalLight& light);

    void SetPointLights(const PointLight* lights, size_t count);
    
    void SetEnvironmentSampler(const std::shared_ptr<Sampler>&);

    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform);
    void DrawModel(const Model& model, const glm::mat4& transform);
}