#pragma once

#include <angelscript.h>

namespace Quanta
{
    class Script;

    class ScriptRuntime final 
    {
        friend class Script; 
    public:
        ScriptRuntime();
        ~ScriptRuntime();
    private:
        asIScriptEngine* engine = nullptr;
        asIScriptContext* context = nullptr;
    };
}