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

        ShaderModule(const ShaderModule&) = delete;

        ShaderModule& operator=(const ShaderModule&) = delete;
        
        uint32_t GetHandle() const;
    private:
        uint32_t handle;
    };
}