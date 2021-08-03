#include <iostream>
#include <glad/glad.h>
#include <Quanta/Graphics/Pipeline/RasterPipeline.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLRasterPipeline.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<RasterPipeline> RasterPipeline::Create(const RasterPipelineDescription& description)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGLRasterPipeline>(description);
        }

        DEBUG_FAILURE_MESSAGE("GraphicsApi not supported");
        
        return nullptr;
    } 

    RasterPipeline::~RasterPipeline() = default;
}