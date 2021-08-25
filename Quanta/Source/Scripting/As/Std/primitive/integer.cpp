#include "integer.h"
#include "../../../../Utilites/Symbols.h"

namespace Quanta::As::Std
{
    void RegisterIntegers(asIScriptEngine* engine)
    {
        engine->RegisterTypedef(nameof(I8), "int8");
        engine->RegisterTypedef(nameof(I16), "int16");
        engine->RegisterTypedef(nameof(I32), "int32");
        engine->RegisterTypedef(nameof(I64), "int64");
        engine->RegisterTypedef(nameof(ISize), "int64");

        engine->RegisterTypedef(nameof(U8), "uint8");
        engine->RegisterTypedef(nameof(U16), "uint16");
        engine->RegisterTypedef(nameof(U32), "uint32");
        engine->RegisterTypedef(nameof(U64), "uint64");
        engine->RegisterTypedef(nameof(USize), "uint64");
        
        engine->RegisterTypedef(nameof(Char), "uint8");
        engine->RegisterTypedef(nameof(Bool), "bool");
    }
}