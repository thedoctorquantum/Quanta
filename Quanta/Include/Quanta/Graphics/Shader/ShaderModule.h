#pragma once

#include <memory>
#include <string>

#include "ShaderType.h"

namespace Quanta
{
    struct ShaderModule
    {
        static std::shared_ptr<ShaderModule> Create(ShaderType type, const std::string& source);

        virtual ~ShaderModule() = default;
    };
}