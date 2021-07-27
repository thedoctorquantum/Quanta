#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Quanta
{
    class AudioSource 
    {
    public:
        static std::shared_ptr<AudioSource> Create();
        
        virtual const glm::vec3& GetTranslation() const = 0;
        virtual void SetTranslation(const glm::vec3& value) = 0;
    };
}