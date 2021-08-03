#include <glad/glad.h>

#include "OpenGLSampler2DArray.h"
#include "OpenGLTexture2DArray.h"
#include "GLEnumConversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLSampler2DArray::OpenGLSampler2DArray(const std::shared_ptr<Texture2DArray>& texture)
    {
        DEBUG_ASSERT(texture != nullptr);
        DEBUG_ASSERT(((OpenGLTexture2DArray*) texture.get()) != nullptr);
        
        this->texture = texture;

        glCreateSamplers(1, &handle);

        DEBUG_ASSERT(handle != 0);
        
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(magnifiaction));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(minification));

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(wrapModeX));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(wrapModeY));
    }
    
    OpenGLSampler2DArray::~OpenGLSampler2DArray()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<Texture2DArray>& OpenGLSampler2DArray::GetTexture() const
    {
        return texture;
    } 

    FilterMode OpenGLSampler2DArray::GetMagnification() const
    {
        return magnifiaction;
    }

    void OpenGLSampler2DArray::SetMagnification(FilterMode value)
    {
        if(magnifiaction == value) return;

        magnifiaction = value;

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(value));
    }

    FilterMode OpenGLSampler2DArray::GetMinification() const
    {
        return minification;
    }

    void OpenGLSampler2DArray::SetMinification(FilterMode value)
    {
        if(minification == value) return;

        minification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(value));
    }

    WrapMode OpenGLSampler2DArray::GetWrapModeX() const
    {
        return wrapModeX;
    }

    void OpenGLSampler2DArray::SetWrapModeX(WrapMode value)
    {
        if(wrapModeX == value) return;

        wrapModeX = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(value));
    }

    WrapMode OpenGLSampler2DArray::GetWrapModeY() const
    {
        return wrapModeY;
    }

    void OpenGLSampler2DArray::SetWrapModeY(WrapMode value)
    {
        if(wrapModeY == value) return;

        wrapModeY = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(value));
    }

    uint32_t OpenGLSampler2DArray::GetHandle() const
    {
        return handle;
    }
}