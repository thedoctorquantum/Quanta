#include <Quanta/CLI/Log/Log.h>
#include <glad/glad.h>
#include <sstream>

#include "Implementation.h"
#include "VertexArray.h"
#include "GraphicsBuffer.h"
#include "RasterPipeline.h"
#include "Texture.h"
#include "Sampler.h"
#include "FrameBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenGL
{
    static void OnDebugMessage(
        const GLenum source,
        const GLenum type,
        const unsigned int id, 
        const GLenum severity, 
        const GLsizei length, 
        const char* const message, 
        const void* const userParam
    ) 
    {
        Log::Level logLevel = Log::Level::Trace;
        const char* severityString = nullptr;

        switch(severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            logLevel = Log::Level::Information;
            severityString = "Notification";

            break;
        case GL_DEBUG_SEVERITY_LOW:
            logLevel = Log::Level::Information;
            severityString = "Low";

            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            logLevel = Log::Level::Warning;
            severityString = "Medium";

            break;
        case GL_DEBUG_SEVERITY_HIGH:
            logLevel = Log::Level::Error;
            severityString = "High";

            break;
        }

        DEBUG_ASSERT(severityString != nullptr);

        std::ostringstream output;

        output << "[GraphicsApi=OpenGL] [";
        output << id;
        output << "] [";
        output << severityString;
        output << "]: ";
        output << message;

        Log::Write(logLevel, output.str());
    }

    Implementation::Implementation(const Window& window)
    {
        DEBUG_ASSERT(window.GetGraphicsApi() == GraphicsApi::OpenGL);
        
        const bool isLoaded = gladLoadGL();

        DEBUG_ASSERT(isLoaded);

        glEnable(GL_MULTISAMPLE);
               
        if constexpr (DEBUG)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 

            glDebugMessageCallback(OnDebugMessage, this);
            
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
        }
        else
        {
            glDisable(GL_DEBUG_OUTPUT);
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        }
    }
    
    void Implementation::ClearBackBuffer(const glm::vec4& color, const float depth, const std::int32_t stencil)
    {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, &color.x);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        glClearNamedFramebufferiv(0, GL_STENCIL, 0, &stencil);
    }
    
    void Implementation::SetRasterPipeline(const Quanta::RasterPipeline* const value)
    {
        if (!value)
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

        const RasterPipeline* glPipeline = nullptr;
        
        if constexpr (DEBUG)
        {
            glPipeline = dynamic_cast<const RasterPipeline*>(value);

            DEBUG_ASSERT(glPipeline != nullptr);
        }
        else
        {
            glPipeline = static_cast<const RasterPipeline*>(value);
        }

        if (rasterPipeline != glPipeline)
        {
            const std::shared_ptr<Quanta::FrameBuffer>& frameBuffer = value->GetFrameBuffer();

            if (frameBuffer != nullptr)
            {
                const OpenGL::FrameBuffer* glFrameBuffer = nullptr;

                if constexpr (DEBUG)
                {
                    glFrameBuffer = dynamic_cast<const OpenGL::FrameBuffer*>(frameBuffer.get());
                }
                else
                {
                    glFrameBuffer = static_cast<const OpenGL::FrameBuffer*>(frameBuffer.get());
                }

                DEBUG_ASSERT(glFrameBuffer != nullptr);

                glBindFramebuffer(GL_FRAMEBUFFER, glFrameBuffer->GetHandle());
            }
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            for (size_t i = 0; i < value->GetUniformBufferCount(); i++)
            {
                const std::shared_ptr<Quanta::GraphicsBuffer>& buffer = value->GetUniformBuffer(i);

                GraphicsBuffer* glBuffer = nullptr;

                if constexpr (DEBUG)
                {
                    glBuffer = dynamic_cast<GraphicsBuffer*>(buffer.get());

                    DEBUG_ASSERT(glBuffer != nullptr);
                }
                else
                {
                    glBuffer = static_cast<GraphicsBuffer*>(buffer.get());
                }

                glBindBufferBase(GL_UNIFORM_BUFFER, i, glBuffer->GetHandle());
            }
            
            for(size_t i = 0; i < value->GetStorageBufferCount(); i++)
            {
                const std::shared_ptr<Quanta::GraphicsBuffer>& buffer = value->GetStorageBuffer(i);

                GraphicsBuffer* glBuffer = nullptr;

                if constexpr (DEBUG)
                {
                    glBuffer = dynamic_cast<GraphicsBuffer*>(buffer.get());
                }
                else
                {
                    glBuffer = static_cast<GraphicsBuffer*>(buffer.get());
                }

                DEBUG_ASSERT(glBuffer != nullptr);

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
    
    void Implementation::SetVertexArray(const Quanta::VertexArray* const value)
    {
        if(vertexArray == value) return;

        if(value == nullptr)
        {
            vertexArray = nullptr;

            glBindVertexArray(0);

            return;
        }

        const VertexArray* glVertexArray = nullptr;

        if constexpr (DEBUG)
        {
            glVertexArray = dynamic_cast<const VertexArray*>(value);

            DEBUG_ASSERT(glVertexArray != nullptr);
        }
        else
        {
            glVertexArray = static_cast<const VertexArray*>(value);
        }

        this->vertexArray = glVertexArray;

        switch(value->GetIndexType())
        {
        case IndexType::UInt8:
            drawElementsType = GL_UNSIGNED_BYTE;
            drawElementSize = sizeof(U8);

            break;
        case IndexType::UInt16:
            drawElementsType = GL_UNSIGNED_SHORT;
            drawElementSize = sizeof(U16);

            break;
        case IndexType::UInt32:
            drawElementsType = GL_UNSIGNED_INT;
            drawElementSize = sizeof(U32);

            break;
        }
        
        glBindVertexArray(glVertexArray->GetHandle());
    }
    
    void Implementation::BindSampler(const Quanta::Sampler* const sampler, const USize index)
    {
        const Sampler* internalSampler = nullptr;
        const Texture* internalTexture = nullptr;
        
        DEBUG_ASSERT(sampler->GetTexture() != nullptr);

        if constexpr (DEBUG)
        {
            internalSampler = dynamic_cast<const Sampler*>(sampler);
            internalTexture = dynamic_cast<const Texture*>(sampler->GetTexture().get());

            DEBUG_ASSERT(internalSampler != nullptr);
            DEBUG_ASSERT(internalTexture != nullptr);
        }
        else
        {
            internalSampler = static_cast<const Sampler*>(sampler);
            internalTexture = static_cast<const Texture*>(sampler->GetTexture().get());
        }

        glBindSampler(index, internalSampler->GetHandle());
        glBindTextureUnit(index, internalTexture->GetHandle());
    }
    
    void Implementation::DispatchDraw(const DrawCommand& command)
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