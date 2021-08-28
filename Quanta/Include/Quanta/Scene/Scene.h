#pragma once

#include <entt/entt.hpp>

namespace Quanta
{
    class Entity;

    class Scene final 
    {
        friend class Entity;
    public:
        Entity Create();
    private:
        entt::registry registry;
    };
}