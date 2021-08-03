#pragma once

#include <memory>

namespace Quanta
{
    class Texture2DArray
    {
    public:
        static std::shared_ptr<Texture2DArray> Create(size_t width, size_t height, size_t depth);

        virtual ~Texture2DArray() = 0;
        
        virtual void SetData(const void* data, size_t index) = 0;
        virtual void GetData(void* data, size_t index) const = 0;
        
        virtual size_t GetWidth() const = 0;
        virtual size_t GetHeight() const = 0;
        virtual size_t GetDepth() const = 0;
    };
}