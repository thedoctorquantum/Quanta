#pragma once

#include <entt/entt.hpp>

#include "Scene.h"

namespace Quanta
{
    class Scene;

    class Entity final 
    {
        friend class Scene;
    public:
        template<typename T>
        T& Get() 
        {
            return scene->registry.get<T>(handle);
        }
        
        template<typename T>
        const T& Get() const 
        {
            return scene->registry.get<T>(handle);
        }
        
        template<typename Type, typename... Args>
        void Set(Args&&... args)
        {
            scene->registry.emplace<Type>(handle, std::forward(args)...);
        }

        operator entt::entity() const;

        bool operator==(Entity) const;
        bool operator!=(Entity) const; 
    private:
        Scene* scene = nullptr; 

        entt::entity handle = entt::null;
    }; 

    template<typename ComponentA, typename ComponentB>
    void Scene::ForEach(const std::function<void(ComponentA, ComponentB)>& function)
    {
        auto view = registry.view<ComponentA, ComponentB>();

        for (auto [a, b] : view.each())
        {
            function(a, b);
        }
    }

/*
    template<typename... Component>
    void Scene::ForEach(const std::function<void(Entity, Component...)>& function)
    {
        const auto view = registry.view<Component...>();

        view.each([&](const entt::entity entityHandle, Component... args)
        {
            Entity entity;

            entity.handle = entityHandle;
            entity.scene = this;

            function(entity, std::forward(args)...);
        });
    }
    */
}