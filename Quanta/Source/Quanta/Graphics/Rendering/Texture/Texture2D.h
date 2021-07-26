#pragma once

#include <stdint.h>
#include <string>
#include <memory>

#include "../../Imaging/Image32.h"

namespace Quanta
{
    class Texture2D final
    {
    public:
        Texture2D(uint32_t width, uint32_t height);
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        
        Texture2D& operator=(Texture2D&) = delete;

        static std::shared_ptr<Texture2D> FromFile(const std::string& filepath);
        
        static std::shared_ptr<Texture2D> FromImage(const Image32& image);
        static std::shared_ptr<Image32> ToImage(const Texture2D& texture);
            
        void SetData(const void* data);
        void GetData(void* data) const;

        uint32_t GetHandle() const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
    private:
        uint32_t handle;
        uint32_t width;
        uint32_t height;
    };
}