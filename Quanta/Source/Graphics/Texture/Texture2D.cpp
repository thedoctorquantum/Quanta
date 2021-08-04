#include <glad/glad.h>
#include <Quanta/Graphics/Texture/Texture2D.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLTexture2D.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<Texture2D> Texture2D::Create(size_t width, size_t height)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height);
        }
        
        return nullptr;
    }
    
    std::shared_ptr<Texture2D> Texture2D::FromFile(const std::string& filepath)
    {
        std::shared_ptr<Image32> image = Image32::FromFile(filepath);

        DEBUG_ASSERT(image != nullptr);
        
        return FromImage(*image);
    }

    std::shared_ptr<Texture2D> Texture2D::FromImage(const Image32& image)
    {
        std::shared_ptr<Texture2D> texture = Create(image.GetWidth(), image.GetHeight());
        
        DEBUG_ASSERT(texture != nullptr);
        DEBUG_ASSERT(image.GetData() != nullptr);

        texture->SetData(image.GetData());

        return texture;
    }

    std::shared_ptr<Image32> Texture2D::ToImage(const Texture2D& texture)
    {
        std::shared_ptr<Image32> image = std::make_shared<Image32>(texture.GetWidth(), texture.GetHeight());

        DEBUG_ASSERT(image != nullptr);
        DEBUG_ASSERT(image->GetData() != nullptr);

        texture.GetData(image->GetData());

        return image; 
    }

    Texture2D::~Texture2D() = default;
}