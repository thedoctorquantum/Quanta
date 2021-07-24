#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include "Texture2D.h"

namespace Quanta
{
    Texture2D::Texture2D(uint32_t width, uint32_t height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &handle);
        
        this->width = width;
        this->height = height;

        glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &handle);
    }
    
    std::shared_ptr<Texture2D> Texture2D::FromFile(const std::string& filepath)
    {
        std::shared_ptr<Image32> image = Image32::FromFile(filepath);

        if(!image)
        {
            return nullptr;
        }

        return FromImage(*image);
    }

    std::shared_ptr<Texture2D> Texture2D::FromImage(const Image32& image)
    {
        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(image.GetWidth(), image.GetHeight());

        texture->SetData(image.GetData());

        return texture;
    }

    std::shared_ptr<Image32> Texture2D::ToImage(const Texture2D& texture)
    {
        std::shared_ptr<Image32> image = std::make_shared<Image32>(texture.GetWidth(), texture.GetHeight());

        texture.GetData(image->GetData());

        return image; 
    }

    void Texture2D::SetData(const void* data)
    {
        glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void Texture2D::GetData(void* data) const
    {
        glGetTextureSubImage(handle, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, width * height, data);
    }

    uint32_t Texture2D::GetHandle() const
    {
        return handle;
    }

    uint32_t Texture2D::GetWidth() const
    {
        return width;
    }

    uint32_t Texture2D::GetHeight() const
    {
        return height;
    }
}