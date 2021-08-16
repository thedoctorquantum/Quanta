#include <iostream>
#include <glad/glad.h>

#include "OpenGLGraphicsDevice.h"
#include "OpenGLVertexArray.h"
#include "OpenGLGraphicsBuffer.h"
#include "OpenGLRasterPipeline.h"
#include "OpenGLTexture.h"
#include "OpenGLSampler.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLGraphicsDevice::OpenGLGraphicsDevice(const Window& window)
    {
        DEBUG_ASSERT(window.GetGraphicsApi() == GraphicsApi::OpenGL);
        
        bool isLoaded = gladLoadGL();

        DEBUG_ASSERT(isLoaded);

        glEnable(GL_MULTISAMPLE);
               
#if DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 

        glDebugMessageCallback([](GLenum source, GLenum type, uint32_t id, GLenum severity, GLsizei length, const char* message, const void* userParam) 
        {
            const OpenGLGraphicsDevice* _this = static_cast<const OpenGLGraphicsDevice*>(userParam);

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
    
    void OpenGLGraphicsDevice::InternalSetRasterPipeline(const RasterPipeline* value)
    {
        if(!value)
        {
            rasterPipeline = nullptr;

            glUseProgram(0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glDisable(GL_DEPTH_TEST);
            glDepthMask(true);
            
            glDisable(GL_SCISSOR_TEST);
            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glFrontFace(GL_CCW);

            return;
        }

        const OpenGLRasterPipeline* glPipeline = nullptr;

#if DEBUG
        glPipeline = dynamic_cast<const OpenGLRasterPipeline*>(value);

        DEBUG_ASSERT(glPipeline != nullptr);
#else
        glPipeline = static_cast<OpenGLRasterPipeline*>(value.get());
#endif

        if(rasterPipeline != glPipeline)
        {
            for(size_t i = 0; i < value->GetUniformBufferCount(); i++)
            {
                const std::shared_ptr<GraphicsBuffer>& buffer = value->GetUniformBuffer(i);

                OpenGLGraphicsBuffer* glBuffer = nullptr;

#if DEBUG
                glBuffer = dynamic_cast<OpenGLGraphicsBuffer*>(buffer.get());

                DEBUG_ASSERT(glBuffer != nullptr);
#else
                glBuffer = static_cast<OpenGLGraphicsBuffer*>(buffer.get());
#endif

                glBindBufferBase(GL_UNIFORM_BUFFER, i, glBuffer->GetHandle());
            }
            
            for(size_t i = 0; i < value->GetStorageBufferCount(); i++)
            {
                const std::shared_ptr<GraphicsBuffer>& buffer = value->GetStorageBuffer(i);

                OpenGLGraphicsBuffer* glBuffer = nullptr;

#if DEBUG
                glBuffer = dynamic_cast<OpenGLGraphicsBuffer*>(buffer.get());

                DEBUG_ASSERT(glBuffer != nullptr);
#else
                glBuffer = static_cast<OpenGLGraphicsBuffer*>(buffer.get());
#endif

                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, glBuffer->GetHandle());
            }

            glUseProgram(glPipeline->GetHandle());
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

        if(value->GetDepthTestMode() != DepthTestMode::None)
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

        if(value->GetBlendMode() != BlendMode::None)
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

        if(value->GetFaceCullMode() != FaceCullMode::None)
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

        rasterPipeline = glPipeline;
    }
    
    void OpenGLGraphicsDevice::InternalSetVertexArray(const VertexArray* value)
    {
        if(vertexArray == value) return;

        if(value == nullptr)
        {
            vertexArray = nullptr;

            glBindVertexArray(0);

            return;
        }

        const OpenGLVertexArray* glVertexArray = nullptr;

#if DEBUG
        glVertexArray = dynamic_cast<const OpenGLVertexArray*>(value);

        DEBUG_ASSERT(glVertexArray != nullptr);
#else
        glVertexArray = static_cast<const OpenGLVertexArray*>(value.get());
#endif

        this->vertexArray = glVertexArray;

        switch(value->GetIndexType())
        {
        case IndexType::UInt8:
            drawElementsType = GL_UNSIGNED_BYTE;
            drawElementSize = sizeof(uint8_t);

            break;
        case IndexType::UInt16:
            drawElementsType = GL_UNSIGNED_SHORT;
            drawElementSize = sizeof(uint16_t);

            break;
        case IndexType::UInt32:
            drawElementsType = GL_UNSIGNED_INT;
            drawElementSize = sizeof(uint32_t);

            break;
        }
        
        glBindVertexArray(glVertexArray->GetHandle());
    }
    
    void OpenGLGraphicsDevice::InternalBindSampler(const Sampler* sampler, size_t index)
    {
        const OpenGLSampler* internalSampler = nullptr;
        const OpenGLTexture* internalTexture = nullptr;

        DEBUG_ASSERT(sampler->GetTexture() != nullptr);

#if DEBUG
        internalSampler = dynamic_cast<const OpenGLSampler*>(sampler);
        internalTexture = dynamic_cast<const OpenGLTexture*>(sampler->GetTexture().get());

        DEBUG_ASSERT(internalSampler != nullptr);
        DEBUG_ASSERT(internalTexture != nullptr);
#else
        internalSampler = static_cast<const PrivateSampler*>(sampler);
        internalTexture = static_cast<const PrivateTexture*>(sampler->GetTexture().get());
#endif

        glBindSampler(index, internalSampler->GetHandle());
        glBindTextureUnit(index, internalTexture->GetHandle());
    }
    
    void OpenGLGraphicsDevice::InternalDispatchDraw(const DrawCommand& command)
    {
        glDrawElementsInstancedBaseVertexBaseInstance(
            geometryLayout, 
            command.Count, 
            drawElementsType, 
            reinterpret_cast<const void*>(command.IndexOffset),
            command.InstanceCount, 
            command.StartVertex,
            command.StartInstance
        );
    }
}