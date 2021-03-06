#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Quanta
{
    struct AudioSource 
    {
        static std::shared_ptr<AudioSource> Create();
        
        virtual ~AudioSource() = default;

        virtual const glm::vec3& GetTranslation() const = 0;
        virtual void SetTranslation(const glm::vec3& value) = 0;
    };
}