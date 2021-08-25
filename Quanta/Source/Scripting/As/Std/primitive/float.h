#pragma once

#include <Quanta/Math/FloatingPoint.h>
#include <angelscript.h>

namespace Quanta::As::Std
{
    using F32 = Float32;
    using F64 = Float64;
    
    void RegisterFloats(asIScriptEngine* engine);
}