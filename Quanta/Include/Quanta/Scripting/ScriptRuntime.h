#pragma once

#include <angelscript.h>

namespace Quanta::ScriptRuntime
{
    void Create();
    void Destroy();
    
    asIScriptEngine* GetEngine();
}