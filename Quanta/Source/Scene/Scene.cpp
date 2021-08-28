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
}