#pragma once 

#include <Quanta/Renderer/Renderer3D.h>
#include <entt/entt.hpp>

namespace Quanta::SceneRenderer
{
    void Render(const entt::registry& scene, bool useSceneCamera = true, Renderer3D::View view = { });
}