#pragma once

#include <Quanta/Graphics/Pipeline/RasterPipeline.h>

namespace Quanta::OpenGL
{
    struct RasterPipeline : public Quanta::RasterPipeline
    {
        explicit RasterPipeline(const RasterPipeline::Description& description);
        ~RasterPipeline();

        RasterPipeline(const RasterPipeline&) = delete;

        RasterPipeline& operator=(const RasterPipeline&) = delete; 

        U32 handle = 0;
    };
}