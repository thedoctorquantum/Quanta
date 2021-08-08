#include <glad/glad.h>

#include "OpenGLSampler.h"
#include "GLEnumConversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLSampler::OpenGLSampler(const std::shared_ptr<Texture>& texture)
    {
        DEBUG_ASSERT(texture != nullptr);
        
        this->texture = texture;

        glCreateSamplers(1, &handle);

        DEBUG_ASSERT(handle != 0);
        
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(magnifiaction));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(minification));

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(wrapModeX));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(wrapModeY));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, WrapModeToGLenum(wrapModeZ));
        
        glSamplerParameteri(handle, GL_TEXTURE_CUBE_MAP_SEAMLESS, 1);
    }
    
    OpenGLSampler::~OpenGLSampler()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<Texture>& OpenGLSampler::GetTexture() const
    {
        return texture;
    } 

    FilterMode OpenGLSampler::GetMagnification() const
    {
        return magnifiaction;
    }

    void OpenGLSampler::SetMagnification(FilterMode value)
    {
        if(magnifiaction == value) return;

        magnifiaction = value;

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(value));
    }

    FilterMode OpenGLSampler::GetMinification() const
    {
        return minification;
    }

    void OpenGLSampler::SetMinification(FilterMode value)
    {
        if(minification == value) return;

        minification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(value));
    }

    WrapMode OpenGLSampler::GetWrapModeX() const
    {
        return wrapModeX;
    }

    void OpenGLSampler::SetWrapModeX(WrapMode value)
    {
        if(wrapModeX == value) return;

        wrapModeX = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(value));
    }

    WrapMode OpenGLSampler::GetWrapModeY() const
    {
        return wrapModeY;
    }

    void OpenGLSampler::SetWrapModeY(WrapMode value)
    {
        if(wrapModeY == value) return;

        wrapModeY = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(value));
    }

    WrapMode OpenGLSampler::GetWrapModeZ() const
    {
        return wrapModeZ;
    }

    void OpenGLSampler::SetWrapModeZ(WrapMode value)
    {
        if(wrapModeZ == value) return;

        wrapModeZ = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, WrapModeToGLenum(value));
    }

    uint32_t OpenGLSampler::GetHandle() const
    {
        return handle;
    }
}