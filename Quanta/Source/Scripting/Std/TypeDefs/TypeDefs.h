#pragma once

#include <Quanta/Math/Integer.h>
#include <Quanta/Math/FloatingPoint.h>
#include <angelscript.h>

namespace Quanta::As_Std
{
    using ::Quanta::Int8;
    using ::Quanta::Int16;
    using ::Quanta::Int32;
    using ::Quanta::Int64;

    using ::Quanta::UInt8;
    using ::Quanta::UInt16;
    using ::Quanta::UInt32;
    using ::Quanta::UInt64;

    using ::Quanta::Char;
    using ::Quanta::WChar;

    using ::Quanta::Float32;
    using ::Quanta::Float64;

    void RegisterTypeDefs(asIScriptEngine* engine);
}