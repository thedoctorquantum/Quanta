#include <Quanta/Graphics/Texture/Texture.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Debugging/Validation.h"
#include "../../Platform/OpenGL/Texture.h"

namespace Quanta
{
    std::shared_ptr<Texture> Texture::Create(TextureType type, TexelFormat format, USize width, USize height, USize depth)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGL::Texture>(type, format, width, height, depth);
        }
        
        return nullptr;
    }
    
    std::shared_ptr<Texture> Texture::Load2D(const std::string& filepath)
    {
        std::shared_ptr<Image32> image = Image32::FromFile(filepath);

        DEBUG_ASSERT(image != nullptr);
        
        return FromImage2D(*image);
    }

    std::shared_ptr<Texture> Texture::FromImage2D(const Image32& image)
    {
        std::shared_ptr<Texture> texture = Create(TextureType::Texture2D, TexelFormat::Rgba8I, image.GetWidth(), image.GetHeight(), 1);

        DEBUG_ASSERT(texture != nullptr);
        DEBUG_ASSERT(image.GetData() != nullptr);

        texture->SetData(image.GetData());

        return texture;
    }
}