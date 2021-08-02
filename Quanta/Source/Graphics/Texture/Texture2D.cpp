#include <glad/glad.h>
#include <iostream>
#include <Quanta/Graphics/Texture/Texture2D.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLTexture2D.h"

namespace Quanta
{
    std::shared_ptr<Texture2D> Texture2D::Create(size_t width, size_t height)
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