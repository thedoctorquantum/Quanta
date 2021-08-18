#include <glad/glad.h>

#include "Sampler.h"
#include "GLEnumConversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGL::Sampler::Sampler(const std::shared_ptr<Quanta::Texture>& texture)
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
    
    OpenGL::Sampler::~Sampler()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<Quanta::Texture>& OpenGL::Sampler::GetTexture() const
    {
        return texture;
    } 

    FilterMode OpenGL::Sampler::GetMagnification() const
    {
        return magnifiaction;
    }

    void OpenGL::Sampler::SetMagnification(FilterMode value)
    {
        if(magnifiaction == value) return;

        magnifiaction = value;

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(value));
    }

    FilterMode OpenGL::Sampler::GetMinification() const
    {
        return minification;
    }

    void OpenGL::Sampler::SetMinification(FilterMode value)
    {
        if(minification == value) return;

        minification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(value));
    }

    WrapMode OpenGL::Sampler::GetWrapModeX() const
    {
        return wrapModeX;
    }

    void OpenGL::Sampler::SetWrapModeX(WrapMode value)
    {
        if(wrapModeX == value) return;

        wrapModeX = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(value));
    }

    WrapMode OpenGL::Sampler::GetWrapModeY() const
    {
        return wrapModeY;
    }

    void OpenGL::Sampler::SetWrapModeY(WrapMode value)
    {
        if(wrapModeY == value) return;

        wrapModeY = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(value));
    }

    WrapMode OpenGL::Sampler::GetWrapModeZ() const
    {
        return wrapModeZ;
    }

    void OpenGL::Sampler::SetWrapModeZ(WrapMode value)
    {
        if(wrapModeZ == value) return;

        wrapModeZ = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, WrapModeToGLenum(value));
    }
    
    UInt32 OpenGL::Sampler::GetHandle() const
    {
        return handle;
    }
}