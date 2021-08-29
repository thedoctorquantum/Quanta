#include <Quanta/Graphics/Pipeline/RasterPipeline.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/RasterPipeline.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<RasterPipeline> RasterPipeline::Create(const RasterPipeline::Description& description)
    {
        const GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);
        
        switch (api)
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGL::RasterPipeline>(description);
        }
        
        return nullptr;
    } 
}