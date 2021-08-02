#pragma once

#include <memory>

namespace Quanta
{
    class Texture3D
    {
    public:
        static std::shared_ptr<Texture3D> Create(size_t width, size_t height, size_t depth);

        virtual ~Texture3D() = 0;

        virtual void SetData(const void* data) = 0;
        virtual void GetData(void* data) const = 0;
        
        virtual size_t GetWidth() const = 0;
        virtual size_t GetHeight() const = 0;
        virtual size_t GetDepth() const = 0;
    };
}