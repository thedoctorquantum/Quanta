#pragma once

#include "../../Math/Integer.h"

namespace Quanta::Log
{
    enum struct Level : U8
    {
        Trace = 0b00000001,
        Information = 0b00000010,
        Debug = 0b00000100, 
        Warning = 0b00001000,
        Error = 0b00010000,
        Fatal = 0b00100000
    };
}