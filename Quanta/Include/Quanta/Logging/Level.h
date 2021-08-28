#pragma once

#include "../Math/Integer.h"

namespace Quanta::Log
{
    enum class Level : U8
    {
        Trace       = 0b00000001,
        Information = 0b00000010,
        Warning     = 0b00000100,
        Error       = 0b00001000
    };
}