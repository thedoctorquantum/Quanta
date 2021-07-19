#pragma once

#include <string>

#include "ShaderType.h"

namespace Quanta
{
    class ShaderModule final
    {
    public:
        ShaderModule(ShaderType type, const std::string& source);
        ~ShaderModule();
        
        uint32_t GetHandle() const;
    private:
        uint32_t handle;
    };
}