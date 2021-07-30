#pragma once

#include <memory>

#include "Texture2D.h"
#include "FilterMode.h"
#include "WrapMode.h"

namespace Quanta
{
    class Sampler2D
    {
    public:
        static std::shared_ptr<Sampler2D> Create(const std::shared_ptr<Texture2D>& texture);
        
        virtual const std::shared_ptr<Texture2D>& GetTexture() const = 0; 
    
        virtual FilterMode GetMagnification() const = 0;
        virtual void SetMagnification(FilterMode value) = 0;
        
        virtual FilterMode GetMinification() const = 0;
        virtual void SetMinification(FilterMode value) = 0;

        virtual WrapMode GetWrapModeX() const = 0;
        virtual void SetWrapModeX(WrapMode value) = 0;

        virtual WrapMode GetWrapModeY() const = 0;
        virtual void SetWrapModeY(WrapMode value) = 0;
    };
}