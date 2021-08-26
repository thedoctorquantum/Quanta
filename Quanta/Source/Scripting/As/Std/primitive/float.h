#pragma once

#include <Quanta/Math/FloatingPoint.h>
#include <angelscript.h>

namespace Quanta::As::Std
{
    using F32 = F32;
    using F64 = F64;
    
    void RegisterFloats(asIScriptEngine* engine);
}