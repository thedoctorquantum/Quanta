#pragma once

#include "Texture.h"
#include "FilterMode.h"
#include "WrapMode.h"

namespace Quanta
{
    class Sampler
    {
    public:
        static std::shared_ptr<Sampler> Create(const std::shared_ptr<Texture>& texture);

        virtual ~Sampler() = default;

        virtual const std::shared_ptr<Texture>& GetTexture() const = 0; 
        
        virtual FilterMode GetMagnification() const = 0;
        virtual void SetMagnification(FilterMode) = 0;
        
        virtual FilterMode GetMinification() const = 0;
        virtual void SetMinification(FilterMode) = 0;

        virtual WrapMode GetWrapModeX() const = 0;
        virtual void SetWrapModeX(WrapMode) = 0;

        virtual WrapMode GetWrapModeY() const = 0;
        virtual void SetWrapModeY(WrapMode) = 0;

        virtual WrapMode GetWrapModeZ() const = 0;
        virtual void SetWrapModeZ(WrapMode) = 0;
    };
};