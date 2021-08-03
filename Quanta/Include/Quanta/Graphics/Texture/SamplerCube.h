#pragma once

#include <cstdint>
#include <memory>

#include "CubeMap.h"
#include "FilterMode.h"
#include "WrapMode.h"

namespace Quanta
{
    class SamplerCube 
    {
    public:
        static std::shared_ptr<SamplerCube> Create(const std::shared_ptr<CubeMap>& texture);
        
        virtual ~SamplerCube() = 0; 

        virtual const std::shared_ptr<CubeMap>& GetTexture() const = 0;
    
        virtual FilterMode GetMagnification() const = 0;
        virtual void SetMagnification(FilterMode value) = 0;
        
        virtual FilterMode GetMinification() const = 0;
        virtual void SetMinification(FilterMode value) = 0;

        virtual WrapMode GetWrapModeX() const = 0;
        virtual void SetWrapModeX(WrapMode value) = 0;

        virtual WrapMode GetWrapModeY() const = 0;
        virtual void SetWrapModeY(WrapMode value) = 0;

        virtual bool GetIsSeamless() const = 0;
        virtual void SetIsSeamless(bool value) = 0;
    };
}