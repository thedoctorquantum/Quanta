#pragma once

#include <memory>

#include "Texture2DArray.h"
#include "FilterMode.h"
#include "WrapMode.h"

namespace Quanta
{
    class Sampler2DArray 
    {
    public:
        static std::shared_ptr<Sampler2DArray> Create(const std::shared_ptr<Texture2DArray>& texture);
        
        virtual ~Sampler2DArray() = 0; 
        
        virtual const std::shared_ptr<Texture2DArray>& GetTexture() const = 0;
    
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