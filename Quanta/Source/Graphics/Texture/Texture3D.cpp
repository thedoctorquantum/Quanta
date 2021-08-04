#include <Quanta/Graphics/Texture/Sampler3D.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLTexture3D.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<Texture3D> Texture3D::Create(size_t width, size_t height, size_t depth)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGLTexture3D>(width, height, depth);
        }

        return nullptr;
    }

    Texture3D::~Texture3D() = default;
}