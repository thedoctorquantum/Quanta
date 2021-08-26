#include "integer.h"
#include "../../../../Utilites/Symbols.h"

namespace Quanta::As::Std
{
    constexpr const char* GetISizeType() 
    {
        if constexpr (sizeof(ISize) == sizeof(I32))
        {
            return "int32";
        }
        else if (sizeof(ISize) == sizeof(I64))
        {
            return "int64";
        }

        return nullptr;
    }   

    constexpr const char* GetUSizeType() 
    {
        if constexpr (sizeof(ISize) == sizeof(U32))
        {
            return "uint32";
        }
        else if (sizeof(ISize) == sizeof(U64))
        {
            return "uint64";
        }

        return nullptr;
    } 
    
    void RegisterIntegers(asIScriptEngine* engine)
    {
        engine->RegisterTypedef(nameof(I8), "int8");
        engine->RegisterTypedef(nameof(I16), "int16");
        engine->RegisterTypedef(nameof(I32), "int32");
        engine->RegisterTypedef(nameof(I64), "int64");
        engine->RegisterTypedef(nameof(ISize), GetISizeType());

        engine->RegisterTypedef(nameof(U8), "uint8");
        engine->RegisterTypedef(nameof(U16), "uint16");
        engine->RegisterTypedef(nameof(U32), "uint32");
        engine->RegisterTypedef(nameof(U64), "uint64");
        engine->RegisterTypedef(nameof(USize), GetUSizeType());
        
        engine->RegisterTypedef(nameof(Char), "uint8");
        engine->RegisterTypedef(nameof(Bool), "bool");
    }
}