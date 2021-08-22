#pragma once

#include <angelscript.h>

namespace Quanta::As_Std
{
    constexpr const char* ScriptNameSpace = "Std";

    void Configure(asIScriptEngine* engine);
}