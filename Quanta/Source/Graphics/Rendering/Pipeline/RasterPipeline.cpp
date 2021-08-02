#include <iostream>
#include <glad/glad.h>
#include <Quanta/Graphics/Rendering/Pipeline/RasterPipeline.h>
#include <Quanta/Graphics/Rendering/GraphicsDevice.h>

#include "../../../Platform/Rendering/OpenGL/OpenGLRasterPipeline.h"

namespace Quanta
{
    std::shared_ptr<RasterPipeline> RasterPipeline::Create(const RasterPipelineDescription& description)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGLRasterPipeline>(description);
        }
        
        return nullptr;
    } 

    RasterPipeline::~RasterPipeline() = default;
}