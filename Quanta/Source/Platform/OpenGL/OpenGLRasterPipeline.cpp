#include <iostream>
#include <glad/glad.h>

#include "OpenGLRasterPipeline.h"
#include "OpenGLShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLRasterPipeline::OpenGLRasterPipeline(const RasterPipelineDescription& description)
    {
        DEBUG_ASSERT(description.ShaderModules.size() >= 2);
        
        this->shaderModules = description.ShaderModules;
        this->uniformBuffers = description.UniformBuffers;
        this->storageBuffers = description.StorageBuffers;
        
        handle = glCreateProgram();

        DEBUG_ASSERT(handle != 0);

        for(const std::shared_ptr<ShaderModule>& shader : shaderModules)
        {
            const OpenGLShaderModule* glShader = nullptr;

#if DEBUG
            glShader = dynamic_cast<const OpenGLShaderModule*>(shader.get());

            DEBUG_ASSERT(glShader != nullptr);
#else
            glShader = static_cast<const OpenGLShaderModule*>(shader.get());
#endif
            
            glAttachShader(handle, glShader->GetHandle());
        }
    
        glLinkProgram(handle);

#if DEBUG
        int success;

        glGetProgramiv(handle, GL_LINK_STATUS, &success);

        if(!success)
        {
            char infoLog[512];

            glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

            DEBUG_FAILURE_MESSAGE(infoLog);
        }
#endif
        
        for(const std::shared_ptr<ShaderModule>& shader : shaderModules)
        {
            const OpenGLShaderModule* glShader = nullptr;

#if DEBUG
            glShader = dynamic_cast<const OpenGLShaderModule*>(shader.get());

            DEBUG_ASSERT(glShader != nullptr);
#else
            glShader = static_cast<const OpenGLShaderModule*>(shader.get());
#endif
            
            glDetachShader(handle, glShader->GetHandle());
        }
    }
    
    OpenGLRasterPipeline::~OpenGLRasterPipeline()
    {
        glDeleteProgram(handle);
    }

    const std::shared_ptr<ShaderModule>& OpenGLRasterPipeline::GetShaderModule(size_t index) const
    {
        DEBUG_ASSERT(index < shaderModules.size());

        return shaderModules[index];
    }
    
    const std::shared_ptr<GraphicsBuffer>& OpenGLRasterPipeline::GetUniformBuffer(size_t index) const
    {
        DEBUG_ASSERT(index < uniformBuffers.size());

        return uniformBuffers[index];
    }
    
    const std::shared_ptr<GraphicsBuffer>& OpenGLRasterPipeline::GetStorageBuffer(size_t index) const
    {
        DEBUG_ASSERT(index < storageBuffers.size());

        return storageBuffers[index];
    }

    const glm::uvec4& OpenGLRasterPipeline::GetViewport() const
    {
        return viewport;
    }

    void OpenGLRasterPipeline::SetViewport(const glm::uvec4& value)
    {
        viewport = value;

        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
    }

    const glm::uvec4& OpenGLRasterPipeline::GetScissorViewport() const
    {
        return scissorViewport;
    }
    
    void OpenGLRasterPipeline::SetScissorViewport(const glm::uvec4& value)
    {
        scissorViewport = value;
    
        glScissor(value.x, value.y, value.z, value.w);
    } 

    size_t OpenGLRasterPipeline::GetShaderModuleCount() const
    {
        return shaderModules.size();
    }
    
    size_t OpenGLRasterPipeline::GetUniformBufferCount() const
    {
        return uniformBuffers.size();
    }
    
    size_t OpenGLRasterPipeline::GetStorageBufferCount() const
    {
        return storageBuffers.size();
    }

    PolygonFillMode OpenGLRasterPipeline::GetPolygonFillMode() const
    {
        return polygonFillMode;
    }

    void OpenGLRasterPipeline::SetPolygonFillMode(PolygonFillMode value)
    {
        polygonFillMode = value;
    }

    DepthTestMode OpenGLRasterPipeline::GetDepthTestMode() const
    {
        return depthTestMode;
    }

    void OpenGLRasterPipeline::SetDepthTestMode(DepthTestMode value)
    {
        depthTestMode = value;
    }

    bool OpenGLRasterPipeline::GetEnableDepthWriting() const
    {
        return enableDepthWriting;
    }

    void OpenGLRasterPipeline::SetEnableDepthWriting(bool value)
    {
        enableDepthWriting = value;
    }

    bool OpenGLRasterPipeline::GetEnableScissorTesting() const
    {
        return enableScissorTesting;
    }

    void OpenGLRasterPipeline::SetEnableScissorTesting(bool value)
    {
        enableScissorTesting = value;
    }

    BlendMode OpenGLRasterPipeline::GetBlendMode() const
    {
        return blendMode;
    }

    void OpenGLRasterPipeline::SetBlendMode(BlendMode value)
    {
        blendMode = value;
    }

    BlendFactor OpenGLRasterPipeline::GetBlendFactor() const
    {
        return blendFactor;
    }
    
    void OpenGLRasterPipeline::SetBlendFactor(BlendFactor value)
    {
        blendFactor = value;
    }

    FaceCullMode OpenGLRasterPipeline::GetFaceCullMode() const
    {
        return faceCullMode;
    }

    void OpenGLRasterPipeline::SetFaceCullMode(FaceCullMode value)
    {
        faceCullMode = value;
    }

    GeometryLayout OpenGLRasterPipeline::GetGeometryLayout() const
    {
        return geometryLayout;
    }

    void OpenGLRasterPipeline::SetGeometryLayout(GeometryLayout value)
    {
        geometryLayout = value;
    }

    GeometryWinding OpenGLRasterPipeline::GetGeometryWinding() const
    {
        return geometryWinding;
    }

    void OpenGLRasterPipeline::SetGeometryWinding(GeometryWinding value)
    {
        geometryWinding = value;
    }

    uint32_t OpenGLRasterPipeline::GetHandle() const
    {
        return handle;
    }
}