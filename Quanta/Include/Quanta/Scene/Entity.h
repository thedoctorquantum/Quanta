#pragma once

#include <memory>
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
    private:
        Scene* scene = nullptr; 

        entt::entity handle;
    }; 
}