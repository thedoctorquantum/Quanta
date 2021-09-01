#pragma once

#include <Quanta/Graphics/Pipeline/RasterPipeline.h>
#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Scene/Components/TransformComponent.h>
#include <Quanta/Scene/Components/PointLightComponent.h>
#include <entt/entt.hpp>

namespace Quanta
{
    class SceneViewer final
    {
    public:
        SceneViewer(entt::registry& scene, const Window& window);

        void Render(const char* title, bool* open = nullptr);

        const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const;
    private:
        std::shared_ptr<FrameBuffer> frameBuffer = nullptr;
        std::shared_ptr<Sampler> frameSampler = nullptr;

        std::shared_ptr<RasterPipeline> entityPipeline = nullptr;

        Renderer3D::View view;

        std::vector<PointLight> pointLights;

        entt::registry& scene;

        entt::entity selectedEntity = entt::null;
    };
}