#pragma once

#include <memory>
#include <string>

#include "Color32.h"

namespace Quanta
{
    class Image32 final
    {
    public:
        Image32(USize width, USize height);
        Image32(Color32* data, USize width, USize height);

        ~Image32();
        
        Image32(Image32&& other) noexcept;

        Image32(const Image32&) = delete;

        Image32& operator=(const Image32&) = delete;
        
        static std::shared_ptr<Image32> FromFile(const std::string& filepath);

        Image32& operator=(Image32&& other) noexcept;

        Color32& operator[](USize index);
        const Color32& operator[](USize index) const;

        Color32& operator()(USize x, USize y);
        const Color32& operator()(USize x, USize y) const;
        
        const Color32* GetData() const;
        Color32* GetData();
        
        USize GetWidth() const;
        USize GetHeight() const;
    private:
        Color32* data = nullptr;

        USize width = 0;
        USize height = 0;
    };
}