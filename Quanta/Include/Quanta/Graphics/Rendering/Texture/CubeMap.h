#pragma once

#include <memory>
#include <vector>

#include "../../Imaging/Image32.h"

namespace Quanta
{
    class CubeMap 
    {
    public:
        static std::shared_ptr<CubeMap> Create(size_t width, size_t height);

        static std::shared_ptr<CubeMap> FromImages(const std::vector<std::shared_ptr<Image32>>& images);

        virtual ~CubeMap() = 0;
        
        virtual void SetData(const void* data, size_t index) = 0;
        virtual void GetData(void* data, size_t index) const = 0;
        
        virtual size_t GetWidth() const = 0;
        virtual size_t GetHeight() const = 0;
    };
}