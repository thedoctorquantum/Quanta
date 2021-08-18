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
        static std::shared_ptr<Texture> Create(TextureType type, TexelFormat format, Size width, Size height, Size depth);

        static std::shared_ptr<Texture> Load2D(const std::string& filepath);

        static std::shared_ptr<Texture> FromImage2D(const Image32& image);

        virtual ~Texture() = default;

        virtual void SetData(const void* data) = 0;
        virtual void SetData(const void* data, Size xOffset, Size yOffset, Size zOffset) = 0;

        virtual TextureType GetType() const = 0;
        virtual TexelFormat GetFormat() const = 0;

        virtual Size GetWidth() const = 0;
        virtual Size GetHeight() const = 0;
        virtual Size GetDepth() const = 0;
    };
}