#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include "Texture2D.h"
#include "../GraphicsDevice.h"
#include "../../../../Platform/Rendering/OpenGL/OpenGLTexture2D.h"

namespace Quanta
{
    std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height);
        }
        
        return nullptr;
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
        std::shared_ptr<Texture2D> texture = Create(image.GetWidth(), image.GetHeight());

        texture->SetData(image.GetData());

        return texture;
    }

    std::shared_ptr<Image32> Texture2D::ToImage(const Texture2D& texture)
    {
        std::shared_ptr<Image32> image = std::make_shared<Image32>(texture.GetWidth(), texture.GetHeight());

        texture.GetData(image->GetData());

        return image; 
    }
}