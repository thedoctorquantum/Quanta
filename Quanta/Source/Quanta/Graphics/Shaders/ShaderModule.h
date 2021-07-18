#pragma once

#include <string>

#include "ShaderType.h"

namespace Quanta
{
    class ShaderModule final
    {
        ShaderModule(ShaderType type, const std::string& source);
    };
}