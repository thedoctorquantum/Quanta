#pragma once 

#include <memory>
#include <vector>

#include "../Shader/ShaderModule.h"
#include "../Buffer/GraphicsBuffer.h"

namespace Quanta
{
    struct RasterPipelineDescription
    {
        std::vector<std::shared_ptr<ShaderModule>> ShaderModules;
        std::vector<std::shared_ptr<GraphicsBuffer>> UniformBuffers;
        std::vector<std::shared_ptr<GraphicsBuffer>> StorageBuffers;
    };   
}