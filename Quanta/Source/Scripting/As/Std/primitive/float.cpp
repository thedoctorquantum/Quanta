#include "float.h"
#include "../../../../Utilites/Symbols.h"

namespace Quanta::As::Std
{
    void RegisterFloats(asIScriptEngine* engine)
    {
        engine->RegisterTypedef(nameof(F32), "float");
        engine->RegisterTypedef(nameof(F64), "double");
    }
}