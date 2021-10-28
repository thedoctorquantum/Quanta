#pragma once

#include <cstdint>

namespace Quanta
{
    struct DrawCommand 
    {
        std::uint32_t Count = 0;
        std::uint32_t InstanceCount = 1;
        std::size_t IndexOffset = 0;
        std::uint32_t StartVertex = 0;
        std::uint32_t StartInstance = 0;
    };
}