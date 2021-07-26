#pragma once

#include <stdint.h>

namespace Quanta
{
    struct DrawCommand final 
    {
        uint32_t Count = 0;
        uint32_t InstanceCount = 1;
        size_t IndexOffset = 0;
        uint32_t StartVertex = 0;
        uint32_t StartInstance = 0;
    };
}