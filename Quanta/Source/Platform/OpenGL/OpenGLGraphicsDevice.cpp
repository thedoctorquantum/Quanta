#include <iostream>
#include <glad/glad.h>

#include "OpenGLGraphicsDevice.h"
#include "OpenGLVertexArray.h"
#include "OpenGLGraphicsBuffer.h"
#include "OpenGLRasterPipeline.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTexture3D.h"
#include "OpenGLSampler2D.h"
#include "OpenGLSampler3D.h"
#include "OpenGLCubeMap.h"
#include "OpenGLSamplerCube.h"

namespace Quanta
{
    static void BindSamplerHandle(uint32_t texture, uint32_t sampler, size_t index)
    {
        glBindTextureUnit(index, texture);
        glBindSampler(index, sampler);
    }

    OpenGLGraphicsDevice::OpenGLGraphicsDevice()
    {
        gladLoadGL();

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int*) &maxTextureSlots);
        
#ifndef NDEBUG
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 

            glDebugMessageCallback([](GLenum source, GLenum type, uint32_t id, GLenum severity, GLsizei length, const char* message, const void* userParam) 
            {
                OpenGLGraphicsDevice* _this = (OpenGLGraphicsDevice*) userParam;
                
                const char* severityString = "Notification";

                switch(severity)
                {
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    severityString = "Notification";

                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    severityString = "Low";

                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    severityString = "Medium";

                    break;
                case GL_DEBUG_SEVERITY_HIGH:
                    severityString = "High";

                    break;
                }

                std::cout << "[GraphicsApi=OpenGL] [" << id << "] [" << severityString << "]: " << message << '\n';
            }, this);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
#else
            glDisable(GL_DEBUG_OUTPUT);
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
#endif
    }
    
    void OpenGLGraphicsDevice::InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, &color.x);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        glClearNamedFramebufferiv(0, GL_STENCIL, 0, &stencil);
    }
    
    void OpenGLGraphicsDevice::InternalSetRasterPipeline(const std::shared_ptr<RasterPipeline>& value)
    {
        if(!value)
        {
            rasterPipeline = nullptr;

            glUseProgram(0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glDisable(GL_DEPTH_TEST);
            glDepthMask(false);
            
            glDisable(GL_SCISSOR_TEST);
            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glFrontFace(GL_CCW);

            return;
        }

        OpenGLRasterPipeline& glPipeline = (OpenGLRasterPipeline&) *value;

        if(rasterPipeline != value.get())
        {
            for(size_t i = 0; i < value->GetUniformBufferCount(); i++)
            {
                const std::shared_ptr<GraphicsBuffer>& buffer = value->GetUniformBuffer(i);

                OpenGLGraphicsBuffer* glBuffer = (OpenGLGraphicsBuffer*) buffer.get();

                glBindBufferBase(GL_UNIFORM_BUFFER, i, glBuffer->GetHandle());
            }

            glUseProgram(glPipeline.GetHandle());
        }

        switch(value->GetPolygonFillMode())
        {
        case PolygonFillMode::Solid:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            break;
        case PolygonFillMode::Wireframe:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            break; 

        case PolygonFillMode::Point:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

            break;
        }

        if((uint8_t) value->GetDepthTestMode())
        {
            glEnable(GL_DEPTH_TEST);

            switch(value->GetDepthTestMode())
            {
            case DepthTestMode::Always:
                glDepthFunc(GL_ALWAYS);
                
                break;
            case DepthTestMode::Never:
                glDepthFunc(GL_NEVER);

                break;
            case DepthTestMode::Equal:
                glDepthFunc(GL_EQUAL);

                break;
            case DepthTestMode::NotEqual:
                glDepthFunc(GL_NOTEQUAL);

                break;
            case DepthTestMode::Greater:
                glDepthFunc(GL_GREATER);

                break;
            case DepthTestMode::GreaterOrEqual:
                glDepthFunc(GL_GEQUAL);

                break;
            case DepthTestMode::Less:
                glDepthFunc(GL_LESS);

                break;
            case DepthTestMode::LessOrEqual:
                glDepthFunc(GL_LEQUAL);

                break;   
            }
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        glDepthMask(value->GetEnableDepthWriting());

        if(value->GetEnableScissorTesting())
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }

        if((uint8_t) value->GetBlendMode())
        {
            glEnable(GL_BLEND);

            switch(value->GetBlendMode())
            {
            case BlendMode::Add:
                glBlendEquation(GL_FUNC_ADD);

                break;
            case BlendMode::Subtract:
                glBlendEquation(GL_FUNC_SUBTRACT);

                break;
            case BlendMode::ReverseSubtract:
                glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);

                break;
            }
        }
        else
        {
            glDisable(GL_BLEND);
        }
        
        switch(value->GetBlendFactor())
        {
        case BlendFactor::Zero:
            glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

            break;
        case BlendFactor::One:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            
            break;
        case BlendFactor::SourceAlpha:
            glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

            break;
        case BlendFactor::InverseSourceAlpha:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            break;
        case BlendFactor::SourceColor:
            glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

            break;
        case BlendFactor::InverseSourceColor:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

            break;
        }

        if((uint8_t) value->GetFaceCullMode())
        {
            glEnable(GL_CULL_FACE);

            switch(value->GetFaceCullMode())
            {
            case FaceCullMode::Back:
                glCullFace(GL_BACK);

                break;
            case FaceCullMode::Front:
                glCullFace(GL_FRONT);
                
                break;
            }
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        
        switch(value->GetGeometryLayout())
        {
        case GeometryLayout::Triangle:
            geometryLayout = GL_TRIANGLES;

            break;
        case GeometryLayout::TriangleStrip:
            geometryLayout = GL_TRIANGLE_STRIP;

            break;
        case GeometryLayout::Line:
            geometryLayout = GL_LINES;

            break;
        case GeometryLayout::Point:
            geometryLayout = GL_POINTS;
            
            break;
        }

        switch(value->GetGeometryWinding())
        {
        case GeometryWinding::CounterClockwise:
            glFrontFace(GL_CCW);

            break;
        case GeometryWinding::Clockise:
            glFrontFace(GL_CW);

            break;
        }

        rasterPipeline = &glPipeline;
    }
    
    void OpenGLGraphicsDevice::InternalSetVertexArray(const std::shared_ptr<VertexArray>& value)
    {
        if(vertexArray == value.get()) return;

        OpenGLVertexArray* openglValue = (OpenGLVertexArray*) value.get();
        
        if(!openglValue)
        {
            vertexArray = nullptr;

            glBindVertexArray(0);

            return;
        }
        
        glBindVertexArray(openglValue->GetHandle());

        this->vertexArray = openglValue;
    }
    
    void OpenGLGraphicsDevice::InternalBindSampler(const Sampler2D& sampler, size_t index)
    {
        const OpenGLSampler2D& glSampler = (const OpenGLSampler2D&) sampler;
        const OpenGLTexture2D& glTexture = (const OpenGLTexture2D&) *sampler.GetTexture();
        
        BindSamplerHandle(glTexture.GetHandle(), glSampler.GetHandle(), index);
    }

    void OpenGLGraphicsDevice::InternalBindSampler(const Sampler3D& sampler, size_t index)
    {
        const OpenGLSampler3D& glSampler = (const OpenGLSampler3D&) sampler;
        const OpenGLTexture3D& glTexture = (const OpenGLTexture3D&) *sampler.GetTexture();

        BindSamplerHandle(glTexture.GetHandle(), glSampler.GetHandle(), index);
    }
    
    void OpenGLGraphicsDevice::InternalBindSampler(const SamplerCube& sampler, size_t index)
    {
        const OpenGLSamplerCube& glSampler = (const OpenGLSamplerCube&) sampler;
        const OpenGLCubeMap& glTexture = (const OpenGLCubeMap&) *sampler.GetTexture();

        BindSamplerHandle(glTexture.GetHandle(), glSampler.GetHandle(), index);
    }

    void OpenGLGraphicsDevice::InternalDispatchDraw(const DrawCommand& command)
    {
        glDrawElementsInstancedBaseVertexBaseInstance(
            geometryLayout, 
            command.Count, 
            vertexArray->GetOpenGLIndexType(), 
            (void*) command.IndexOffset,
            command.InstanceCount, 
            command.StartVertex,
            command.StartInstance);
    }
}