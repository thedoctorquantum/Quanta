#include <Quanta/Scene/Entity.h>

namespace Quanta
{
    Entity::operator entt::entity() const
    {
        return handle;
    }

    bool Entity::operator==(const Entity other) const
    {
        return handle == other.handle;
    }
    
    bool Entity::operator!=(const Entity other) const
    {
        return !(*this == other);
    }
}