#pragma once

#include <Quanta/Graphics/Texture/FilterMode.h>
#include <Quanta/Graphics/Texture/WrapMode.h>
#include <glad/glad.h>

namespace Quanta
{
    GLenum FilterModeToGLenum(FilterMode mode);
    GLenum WrapModeToGLenum(WrapMode mode);
}