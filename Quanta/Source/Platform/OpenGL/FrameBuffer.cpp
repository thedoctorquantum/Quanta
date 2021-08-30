#include <Quanta/Logging/Log.h>
#include <glad/glad.h>

#include "FrameBuffer.h"
#include "Texture.h"
#include "Conversions.h"
#include "../../Debugging/Validation.h"
#include "../../Utilites/Symbols.h"

namespace Quanta
{
    static std::uint32_t GetTextureHandle(const std::shared_ptr<Texture>& texture)
    {
        const OpenGL::Texture* glTexture = nullptr;

        if constexpr (DEBUG)
        {
            glTexture = dynamic_cast<OpenGL::Texture*>(texture.get());

            DEBUG_ASSERT(glTexture != nullptr);
        }
        else
        {
            glTexture = static_cast<OpenGL::Texture*>(texture.get());
        }

        return glTexture->GetHandle(); 
    }

    OpenGL::FrameBuffer::FrameBuffer(const Description& description)
    {
        DEBUG_ASSERT(description.width != 0);
        DEBUG_ASSERT(description.height != 0);

        this->width = description.width;
        this->height = description.height;

        glCreateFramebuffers(1, &handle);

        DEBUG_ASSERT(handle != 0);

        std::vector<GLuint> drawbuffers;

        for (std::size_t i = 0; i < description.colorAttachments.size(); i++)
        {
            const AttachmentDescription& attachmentDescription = description.colorAttachments[i];

            const std::shared_ptr<Quanta::Texture> texture = Quanta::Texture::Create(
                TextureType::Texture2D,
                attachmentDescription.format, 
                description.width,
                description.height,
                1
            );

            const GLuint glAttachment = GL_COLOR_ATTACHMENT0 + i;

            glNamedFramebufferTexture(handle, glAttachment, GetTextureHandle(texture), 0);

            drawbuffers.push_back(glAttachment);

            colorTextures.push_back(texture);  
        }

        depthTexture = Quanta::Texture::Create(
            TextureType::Texture2D,
            description.depthAttachment.format, 
            description.width,
            description.height,
            1
        );
        
        glNamedFramebufferTexture(handle, GL_DEPTH_STENCIL_ATTACHMENT, GetTextureHandle(depthTexture), 0);

        glNamedFramebufferDrawBuffers(handle, drawbuffers.size(), drawbuffers.data());

        DEBUG_ASSERT(glCheckNamedFramebufferStatus(handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    OpenGL::FrameBuffer::~FrameBuffer()
    {
        DEBUG_ASSERT(handle != 0);

        glDeleteFramebuffers(1, &handle);
    }

    size_t OpenGL::FrameBuffer::GetWidth() const
    {
        return width;
    }

    size_t OpenGL::FrameBuffer::GetHeight() const
    {
        return height;
    }
    
    void OpenGL::FrameBuffer::ClearColorTexture(const size_t index, const glm::vec4& color)
    {
        DEBUG_ASSERT(index < colorTextures.size());

        const std::shared_ptr<Quanta::Texture>& texture = colorTextures[index];

        DEBUG_ASSERT(texture != nullptr);
        
        glClearTexImage(
            GetTextureHandle(texture),
            0, 
            TexelFormatToInternalFormat(texture->GetFormat()),
            TexelFormatToPixelType(texture->GetFormat()),
            &color
        );
    }
    
    void OpenGL::FrameBuffer::ClearDepthTexture(const float depth)
    {
        glClearTexImage(
            GetTextureHandle(depthTexture),
            0, 
            TexelFormatToInternalFormat(depthTexture->GetFormat()),
            TexelFormatToPixelType(depthTexture->GetFormat()),
            &depth
        );
    }
    
    void OpenGL::FrameBuffer::Clear(const glm::vec4& color, const float depth, const std::int32_t stencil) 
    {
        glClearNamedFramebufferfv(handle, GL_COLOR, 0, &color.x);
        glClearNamedFramebufferfv(handle, GL_DEPTH, 0, &depth);
        glClearNamedFramebufferiv(handle, GL_STENCIL, 0, &stencil);
    }

    std::shared_ptr<Quanta::Texture> OpenGL::FrameBuffer::GetColorTexture(const size_t index) const
    {
        DEBUG_ASSERT(index < colorTextures.size());

        return colorTextures[index];
    }
    
    std::shared_ptr<Quanta::Texture> OpenGL::FrameBuffer::GetDepthTexture() const
    {
        return depthTexture;
    } 

    std::uint32_t OpenGL::FrameBuffer::GetHandle() const 
    {
        return handle;
    }
}