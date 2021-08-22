#pragma once

#include <Quanta/Graphics/Texture/FilterMode.h>
#include <Quanta/Graphics/Texture/WrapMode.h>
#include <Quanta/Graphics/Texture/TexelFormat.h>
#include <glad/glad.h>

namespace Quanta
{
    GLenum FilterModeToGLenum(FilterMode);
    GLenum WrapModeToGLenum(WrapMode);
    
    GLenum TexelFormatToPixelFormat(TexelFormat);
    GLenum TexelFormatToInternalFormat(TexelFormat);
    GLenum TexelFormatToPixelType(TexelFormat);
}