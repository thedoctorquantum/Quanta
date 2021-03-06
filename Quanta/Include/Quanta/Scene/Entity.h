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
        Type& Set(Args&&... args)
        {
            return scene->registry.emplace<Type>(handle, std::forward(args)...);
        }

        operator entt::entity() const;

        bool operator==(Entity) const;
        bool operator!=(Entity) const; 
    private:
        Scene* scene = nullptr; 

        entt::entity handle = entt::null;
    }; 
}