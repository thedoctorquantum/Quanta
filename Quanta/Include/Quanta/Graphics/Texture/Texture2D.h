#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "../Imaging/Image32.h"

namespace Quanta
{
    class Texture2D
    {
    public:
        static std::shared_ptr<Texture2D> Create(size_t width, size_t height);

        static std::shared_ptr<Texture2D> FromFile(const std::string& filepath);
        
        static std::shared_ptr<Texture2D> FromImage(const Image32& image);
        static std::shared_ptr<Image32> ToImage(const Texture2D& texture);
        
        virtual ~Texture2D() = 0;

        virtual void SetData(const void* data) = 0;
        virtual void GetData(void* data) const = 0;
        
        virtual size_t GetWidth() const = 0;
        virtual size_t GetHeight() const = 0;
    };
}