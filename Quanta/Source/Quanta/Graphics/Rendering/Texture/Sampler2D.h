#pragma once

#include <memory>

#include "Texture2D.h"

namespace Quanta
{
    class Sampler2D
    {
    public:
        static std::shared_ptr<Sampler2D> Create(const std::shared_ptr<Texture2D>& texture);
        
        virtual const std::shared_ptr<Texture2D>& GetTexture() const = 0; 
    };
}