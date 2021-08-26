#pragma once

#include <memory>
#include <string>

#include "TextureType.h"
#include "TexelFormat.h"
#include "../Imaging/Image32.h"

namespace Quanta
{
    class Texture
    {
    public:
        static std::shared_ptr<Texture> Create(TextureType type, TexelFormat format, USize width, USize height, USize depth);

        static std::shared_ptr<Texture> Load2D(const std::string& filepath);

        static std::shared_ptr<Texture> FromImage2D(const Image32& image);

        virtual ~Texture() = default;

        virtual void SetData(const void* data) = 0;
        virtual void SetData(const void* data, USize xOffset, USize yOffset, USize zOffset) = 0;

        virtual TextureType GetType() const = 0;
        virtual TexelFormat GetFormat() const = 0;

        virtual USize GetWidth() const = 0;
        virtual USize GetHeight() const = 0;
        virtual USize GetDepth() const = 0;
    };
}