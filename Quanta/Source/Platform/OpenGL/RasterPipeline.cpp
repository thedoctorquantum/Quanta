#include <Quanta/CLI/Log/Log.h>
#include <glad/glad.h>

#include "RasterPipeline.h"
#include "ShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenGL
{
    RasterPipeline::RasterPipeline(const RasterPipeline::Description& description)
    {        
        uniformBuffers = description.uniformBuffers;
        storageBuffers = description.storageBuffers;
        framebuffer = description.frameBuffer;
        
        handle = glCreateProgram();

        DEBUG_ASSERT(handle != 0);

        vertexShader = description.vertexShader;
        fragmentShader = description.fragmentShader;

        const ShaderModule* glVertexShader = nullptr;
        const ShaderModule* glFragmentShader = nullptr;

        if constexpr (DEBUG)
        {
            glVertexShader = dynamic_cast<const ShaderModule*>(vertexShader.get());
            glFragmentShader = dynamic_cast<const ShaderModule*>(fragmentShader.get());

            DEBUG_ASSERT(vertexShader != nullptr);
            DEBUG_ASSERT(fragmentShader != nullptr);
        }
        else
        {
            glVertexShader = static_cast<const ShaderModule*>(vertexShader.get());
            glFragmentShader = static_cast<const ShaderModule*>(fragmentShader.get());
        }

        glAttachShader(handle, glVertexShader->handle);
        glAttachShader(handle, glFragmentShader->handle);

        glLinkProgram(handle);

        glDetachShader(handle, glVertexShader->handle);
        glDetachShader(handle, glFragmentShader->handle);

        if constexpr (DEBUG)
        {
            int success = 0;

            glGetProgramiv(handle, GL_LINK_STATUS, &success);

            if (!success)
            {
                char infoLog[512];

                glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

                Log::Write(Log::Level::Fatal, infoLog);

                DEBUG_FAILURE();
            }
        }
    }
    
    RasterPipeline::~RasterPipeline()
    {
        glDeleteProgram(handle);
    }
}