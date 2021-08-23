#pragma once

#include <angelscript.h>
#include <string>

#include "../TypeDefs/TypeDefs.h"

namespace Quanta::As_Std
{
    void print(const std::string& string);
    void putchar(char character);
    void _throw(const std::string& message);
    std::string get_exception();

    void RegisterFunctions(asIScriptEngine* engine);
}