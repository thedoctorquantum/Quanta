#include "TypeDefs.h"
#include "../../../Utilites/Symbols.h"

namespace Quanta::As_Std
{
    void RegisterTypeDefs(asIScriptEngine* engine)
    {
        engine->RegisterTypedef(nameof(i8), "int8");
        engine->RegisterTypedef(nameof(i16), "int16");
        engine->RegisterTypedef(nameof(i32), "int32");
        engine->RegisterTypedef(nameof(i64), "int64");
        engine->RegisterTypedef(nameof(isize), "int64");

        engine->RegisterTypedef(nameof(u8), "uint8");
        engine->RegisterTypedef(nameof(u16), "uint16");
        engine->RegisterTypedef(nameof(u32), "uint32");
        engine->RegisterTypedef(nameof(u64), "uint64");
        engine->RegisterTypedef(nameof(usize), "uint64");

        engine->RegisterTypedef(nameof(f32), "float");
        engine->RegisterTypedef(nameof(f64), "double");

        engine->RegisterTypedef(nameof(char), "int8");
    }
}