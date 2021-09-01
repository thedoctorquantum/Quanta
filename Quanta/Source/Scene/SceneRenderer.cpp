#include <Quanta/Scene/SceneRenderer.h>
#include <Quanta/Scene/Components/TransformComponent.h>
#include <Quanta/Scene/Components/CameraComponent.h>
#include <Quanta/Scene/Components/ModelRendererComponent.h>
#include <Quanta/Scene/Components/PointLightComponent.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Quanta::SceneRenderer
{
    struct 
    {
        std::vector<PointLight> pointLights;
    } static state;

    void Render(const entt::registry& scene, const bool useSceneCamera, Renderer3D::View view)
    {
        const auto pointLights = scene.view<TransformComponent, PointLightComponent>();
        const auto models = scene.view<TransformComponent, ModelRendererComponent>();

        if (useSceneCamera)
        {
            const auto cameras = scene.view<TransformComponent, CameraComponent>();
            
            const entt::entity mainCamera = cameras.front();
    
            if (mainCamera != entt::null)
            {
                const TransformComponent& transform = scene.get<TransformComponent>(mainCamera);
                const CameraComponent& camera = scene.get<CameraComponent>(mainCamera);

                view.width = camera.width;
                view.height = camera.height;
                view.near = camera.near;
                view.far = camera.far;
                view.fieldOfView = camera.fieldOfView;

                view.matrix = glm::lookAt(transform.translation, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
            }
        }

        state.pointLights.clear();
        
        for (const auto [entity, transform, light] : pointLights.each())
        {
            PointLight lightData; 

            lightData.Position = transform.translation;
            lightData.Ambient = light.ambient;
            lightData.Diffuse = light.diffuse;
            lightData.Specular = light.specular;
            lightData.Linear = light.linear;
            lightData.Quadratic = light.quadratic;

            state.pointLights.push_back(lightData);
        }
        
        Renderer3D::SetPointLights(state.pointLights.data(), state.pointLights.size());

        Renderer3D::BeginPass(view);

        for (const auto [entity, transform, modelRenderer] : models.each())
        {
            const glm::mat4 transformMatrix = transform.CreateMatrix();

            const Model& model = *modelRenderer.model;

            Renderer3D::DrawModel(model, transformMatrix);
        }

        Renderer3D::EndPass();
    }   
}