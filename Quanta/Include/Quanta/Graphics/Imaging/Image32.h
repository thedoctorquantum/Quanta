#pragma once

#include <memory>
#include <string>

#include "Color32.h"

namespace Quanta
{
    class Image32 final
    {
    public:
        Image32(Size width, Size height);
        Image32(Color32* data, Size width, Size height);

        ~Image32();
        
        Image32(Image32&& other) noexcept;

        Image32(const Image32&) = delete;

        Image32& operator=(const Image32&) = delete;
        
        static std::shared_ptr<Image32> FromFile(const std::string& filepath);

        Image32& operator=(Image32&& other) noexcept;

        Color32& operator[](Size index);
        const Color32& operator[](Size index) const;

        Color32& operator()(Size x, Size y);
        const Color32& operator()(Size x, Size y) const;
        
        const Color32* GetData() const;
        Color32* GetData();
        
        Size GetWidth() const;
        Size GetHeight() const;
    private:
        Color32* data = nullptr;

        Size width = 0;
        Size height = 0;
    };
}