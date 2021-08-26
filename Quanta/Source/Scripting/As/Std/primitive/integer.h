#pragma once

#include <Quanta/Math/Integer.h>
#include <angelscript.h>

namespace Quanta::As::Std
{
    using I8 = I8;
    using I16 = I16;
    using I32 = I32;
    using I64 = I64;
    using ISize = __ssize_t;

    using U8 = U8;
    using U16 = U16;
    using U32 = U32;
    using U64 = U64;
    using USize = USize;
    
    using Char = char;
    using Bool = bool;

    void RegisterIntegers(asIScriptEngine* engine);
}