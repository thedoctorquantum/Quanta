#include <glad/glad.h>

#include "FrameBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGL::FrameBuffer::FrameBuffer(const Description& description)
    {
        this->width = description.width;
        this->height = description.height;

        glCreateFramebuffers(1, &handle);
    }

    OpenGL::FrameBuffer::~FrameBuffer()
    {
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

    void OpenGL::FrameBuffer::ClearTexture(size_t index, const glm::vec4& color)
    {
        
    }

    std::shared_ptr<Texture> OpenGL::FrameBuffer::GetTexture(size_t index) const
    {
        return nullptr;
    }
}