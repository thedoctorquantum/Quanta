#pragma once

#include <angelscript.h>
#include <string>

#include "TypeDefs.h"

namespace Quanta::As_Std
{
    void Print(const std::string& string);
    void PutChar(Char character);

    void RegisterFunctions(asIScriptEngine* engine);
}