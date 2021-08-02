#include <Quanta/Graphics/Texture/CubeMap.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLCubeMap.h"

namespace Quanta
{
    std::shared_ptr<CubeMap> CubeMap::Create(size_t width, size_t height)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLCubeMap>(width, height);
        }

        return nullptr;
    }
    
    std::shared_ptr<CubeMap> CubeMap::FromImages(const std::vector<std::shared_ptr<Image32>>& images)
    {
        std::shared_ptr<CubeMap> cubeMap = Create(images[0]->GetWidth(), images[0]->GetHeight());

        for(size_t i = 0; i < images.size(); i++)
        {
            cubeMap->SetData(images[i]->GetData(), i);
        }

        return cubeMap;
    }

    CubeMap::~CubeMap() = default;
}