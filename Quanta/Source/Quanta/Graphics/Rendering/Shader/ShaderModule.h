#pragma once

#include <memory>
#include <string>

#include "ShaderType.h"

namespace Quanta
{
    class ShaderModule
    {
    public:
        static std::shared_ptr<ShaderModule> Create(ShaderType type, const std::string& source);
    };
}