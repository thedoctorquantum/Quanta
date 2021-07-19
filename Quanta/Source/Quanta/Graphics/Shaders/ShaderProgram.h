#pragma once

#include <memory>
#include <vector>
#include "ShaderModule.h"

namespace Quanta
{
    class ShaderProgram final
    {
    public:
        ShaderProgram(const std::vector<std::shared_ptr<ShaderModule>>& modules);
        ~ShaderProgram();
        
        uint32_t GetHandle() const;
    private:
        uint32_t handle = 0;
    };
}