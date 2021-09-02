#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class Permission : U8
    {
        Low = 0b00000001,
        Medium = 0b00000010,
        High = 0b00000100
    };
}