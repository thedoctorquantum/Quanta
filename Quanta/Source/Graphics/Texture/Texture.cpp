#include <Quanta/Graphics/Texture/Texture.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Debugging/Validation.h"
#include "../../Platform/OpenGL/Texture.h"

namespace Quanta
{
    std::shared_ptr<Texture> Texture::Create(const Type type, const TexelFormat format, const USize width, const USize height, const USize depth)
    {
        const auto api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch (api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGL::Texture>(type, format, width, height, depth);
        }
        
        return nullptr;
    }
    
    std::shared_ptr<Texture> Texture::Load2D(const std::string& filepath)
    {
        const auto image = Image32::FromFile(filepath);

        DEBUG_ASSERT(image != nullptr);
        
        return FromImage2D(*image);
    }
    
    std::shared_ptr<Texture> Texture::FromImage2D(const Image32& image)
    {
        const auto texture = Create(Type::Texture2D, TexelFormat::Rgba8I, image.GetWidth(), image.GetHeight(), 1);

        DEBUG_ASSERT(texture != nullptr);
        DEBUG_ASSERT(image.GetData() != nullptr);

        texture->SetData(image.GetData());

        return texture;
    }
}