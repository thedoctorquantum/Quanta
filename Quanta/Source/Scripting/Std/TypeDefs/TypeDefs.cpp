#include "TypeDefs.h"
#include "../../../Utilites/Symbols.h"

namespace Quanta::As_Std
{
    void RegisterTypeDefs(asIScriptEngine* engine)
    {
        engine->RegisterTypedef(nameof(Int8), "int8");
        engine->RegisterTypedef(nameof(Int16), "int16");
        engine->RegisterTypedef(nameof(Int32), "int32");
        engine->RegisterTypedef(nameof(Int64), "int64");

        engine->RegisterTypedef(nameof(UInt8), "uint8");
        engine->RegisterTypedef(nameof(UInt16), "uint16");
        engine->RegisterTypedef(nameof(UInt32), "uint32");
        engine->RegisterTypedef(nameof(UInt64), "uint64");

        engine->RegisterTypedef(nameof(Float32), "float");
        engine->RegisterTypedef(nameof(Float64), "float");

        engine->RegisterTypedef(nameof(Char), "int8");
        engine->RegisterTypedef(nameof(WChar), "int16");
    }
}