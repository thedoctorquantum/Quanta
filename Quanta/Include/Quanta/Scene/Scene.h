#pragma once

#include <functional>
#include <entt/entt.hpp>

namespace Quanta
{
    class Entity;

    class Scene final 
    {
        friend class Entity;
    public:
        Entity Create();
        void Destroy(Entity entity);

        void ForEach(const std::function<void(Entity)>& function);

        template<typename ComponentA, typename ComponentB>
        void ForEach(const std::function<void(ComponentA, ComponentB)>& function);

        const entt::registry& GetRegistry() const;
        entt::registry& GetRegistry();
    private:
        entt::registry registry;
    };
}