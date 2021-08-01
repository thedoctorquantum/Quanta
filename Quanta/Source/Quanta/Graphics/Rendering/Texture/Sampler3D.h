#pragma once

#include <cstdint>
#include <memory>

#include "Texture3D.h"
#include "FilterMode.h"
#include "WrapMode.h"

namespace Quanta
{
    class Sampler3D 
    {
    public:
        static std::shared_ptr<Sampler3D> Create(const std::shared_ptr<Texture3D>& texture);
        
        virtual const std::shared_ptr<Texture3D>& GetTexture() const = 0;

        virtual ~Sampler3D() = 0; 
    
        virtual FilterMode GetMagnification() const = 0;
        virtual void SetMagnification(FilterMode value) = 0;
        
        virtual FilterMode GetMinification() const = 0;
        virtual void SetMinification(FilterMode value) = 0;

        virtual WrapMode GetWrapModeX() const = 0;
        virtual void SetWrapModeX(WrapMode value) = 0;

        virtual WrapMode GetWrapModeY() const = 0;
        virtual void SetWrapModeY(WrapMode value) = 0;

        virtual WrapMode GetWrapModeZ() const = 0;
        virtual void SetWrapModeZ(WrapMode value) = 0;
    };
}