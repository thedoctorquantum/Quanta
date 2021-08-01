#pragma once

#include <memory>
#include <string>

#include "Color32.h"

namespace Quanta
{
    class Image32 final
    {
    public:
        Image32(size_t width, size_t height);
        Image32(Color32* data, size_t width, size_t height);

        ~Image32();
        
        Image32(Image32&& other) noexcept;

        Image32(const Image32&) = delete;

        Image32& operator=(const Image32&) = delete;
        
        static std::shared_ptr<Image32> FromFile(const std::string& filepath);

        Image32& operator=(Image32&& other) noexcept;

        Color32& operator[](size_t index);
        const Color32& operator[](size_t index) const;

        Color32& operator()(size_t x, size_t y);
        const Color32& operator()(size_t x, size_t y) const;
        
        const Color32* GetData() const;
        Color32* GetData();
        
        size_t GetWidth() const;
        size_t GetHeight() const;
    private:
        Color32* data;

        size_t width;
        size_t height;
    };
}