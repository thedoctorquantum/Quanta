#include <Quanta/Graphics/Imaging/Image32.h>
#include <stb_image.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    Image32::Image32(Size width, Size height)
    {
        DEBUG_ASSERT(width != 0);
        DEBUG_ASSERT(height != 0);

        data = new Color32[width * height];

        this->width = width;
        this->height = height;
    }

    Image32::Image32(Color32* data, Size width, Size height)
    {
        DEBUG_ASSERT(data != nullptr);   
        DEBUG_ASSERT(width != 0);
        DEBUG_ASSERT(height != 0);

        this->data = data;
        this->width = width;
        this->height = height;
    }

    Image32::~Image32()
    {
        delete[] data;
    }

    Image32::Image32(Image32&& other) noexcept
    {
        if(this == &other) return;

        data = other.data;
        width = other.width;
        height = other.height;

        other.data = nullptr;
        other.width = 0;
        other.height = 0;
    }
    
    std::shared_ptr<Image32> Image32::FromFile(const std::string& filepath)
    {
        int width;
        int height;
        int channels;

        stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        
        DEBUG_ASSERT(data != nullptr);
        
        return std::make_shared<Image32>(reinterpret_cast<Color32*>(data), width, height);
    }

    Image32& Image32::operator=(Image32&& other) noexcept
    {
        if(this == &other) return *this;

        delete[] data;

        data = other.data;
        width = other.width;
        height = other.height;
        
        other.data = nullptr;
        other.width = 0;
        other.height = 0;

        return *this;
    }

    Color32& Image32::operator[](Size index)
    {
        DEBUG_ASSERT(index < width * height);

        return data[index];
    }
    
    const Color32& Image32::operator[](Size index) const
    {
        DEBUG_ASSERT(index < width * height);

        return data[index];
    }

    Color32& Image32::operator()(Size x, Size y)
    {
        DEBUG_ASSERT(x < width);
        DEBUG_ASSERT(y < height);

        return data[x + width * y];
    }
    
    const Color32& Image32::operator()(Size x, Size y) const
    {
        return this->operator()(x, y);
    }
    
    const Color32* Image32::GetData() const
    {
        return data;
    }
    
    Color32* Image32::GetData()
    {
        return data;
    }
    
    Size Image32::GetWidth() const
    {
        return width;
    }

    Size Image32::GetHeight() const
    {
        return height;
    }
}