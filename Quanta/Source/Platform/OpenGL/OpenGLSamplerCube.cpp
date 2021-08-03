#include <glad/glad.h>

#include "OpenGLSamplerCube.h"
#include "GLEnumConversions.h"

namespace Quanta
{
    OpenGLSamplerCube::OpenGLSamplerCube(const std::shared_ptr<CubeMap>& texture)
    {
        this->texture = texture;

        glCreateSamplers(1, &handle);

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(magnification));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(minification));

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(wrapModeX));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(wrapModeY));
    }
    
    OpenGLSamplerCube::~OpenGLSamplerCube()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<CubeMap>& OpenGLSamplerCube::GetTexture() const 
    {
        return texture;
    }
    
    FilterMode OpenGLSamplerCube::GetMagnification() const 
    {
        return magnification;
    }

    void OpenGLSamplerCube::SetMagnification(FilterMode value) 
    {
        if(magnification == value) return;

        magnification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(value));
    }
    
    FilterMode OpenGLSamplerCube::GetMinification() const
    {
        return minification;
    }

    void OpenGLSamplerCube::SetMinification(FilterMode value)
    {
        if(minification == value) return;

        minification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(value));
    }
    
    WrapMode OpenGLSamplerCube::GetWrapModeX() const
    {
        return wrapModeX;
    }
    
    void OpenGLSamplerCube::SetWrapModeX(WrapMode value) 
    {
        if(wrapModeX == value) return;

        wrapModeX = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(value));
    }
    
    WrapMode OpenGLSamplerCube::GetWrapModeY() const 
    {
        return wrapModeY;
    }
    
    void OpenGLSamplerCube::SetWrapModeY(WrapMode value) 
    {
        if(wrapModeY == value) return;

        wrapModeY = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(value));
    }
    
    bool OpenGLSamplerCube::GetIsSeamless() const
    {
        return isSeamless;
    }
    
    void OpenGLSamplerCube::SetIsSeamless(bool value) 
    {
        if(isSeamless == value) return;

        isSeamless = value;

        glSamplerParameteri(handle, GL_TEXTURE_CUBE_MAP_SEAMLESS, value);
    }

    uint32_t OpenGLSamplerCube::GetHandle() const
    {
        return handle;
    }
}