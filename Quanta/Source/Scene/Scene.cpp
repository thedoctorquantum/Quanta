#include <Quanta/Scene/Scene.h>
#include <Quanta/Scene/Entity.h>

namespace Quanta
{
    Entity Scene::Create() 
    {
        Entity entity;

        entity.scene = this;
        entity.handle = registry.create();

        return entity;
    }

    void Scene::Destroy(const Entity entity)
    {
        registry.destroy(entity.handle);
    }

    void Scene::ForEach(const std::function<void(Entity)>& function)
    {
        registry.each([&](const auto entityHandle)
        {
            Entity entity;

            entity.handle = entityHandle;
            entity.scene = this;

            function(entity);
        });
    }
    
    const entt::registry& Scene::GetRegistry() const
    {
        return registry;
    }

    entt::registry& Scene::GetRegistry()
    {
        return registry;
    }
}