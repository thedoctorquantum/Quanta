#pragma once

#include <Quanta/Math/Integer.h>
#include <Quanta/Math/FloatingPoint.h>
#include <angelscript.h>

namespace Quanta::As_Std
{
    using i8 = Int8;
    using i16 = Int16;
    using i32 = Int32;
    using i64 = Int64;
    using isize = ssize_t;

    using u8 = UInt8;
    using u16 = UInt16;
    using u32 = UInt32;
    using u64 = UInt64;
    using usize = Size;

    using f32 = Float32;
    using f64 = Float64;
        
    void RegisterTypeDefs(asIScriptEngine* engine);
}