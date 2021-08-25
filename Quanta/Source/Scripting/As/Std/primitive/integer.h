#pragma once

#include <Quanta/Math/Integer.h>
#include <angelscript.h>

namespace Quanta::As::Std
{
    using I8 = Int8;
    using I16 = Int16;
    using I32 = Int32;
    using I64 = Int64;
    using ISize = __ssize_t;

    using U8 = UInt8;
    using U16 = UInt16;
    using U32 = UInt32;
    using U64 = UInt64;
    using USize = Size;
    
    using Char = char;
    using Bool = bool;

    void RegisterIntegers(asIScriptEngine* engine);
}