#pragma once

#include <memory>
#include <angelscript.h>

#include "../Math/Integer.h"

namespace Quanta
{
    class ScriptRuntime;

    class Script final
    {
        friend class ScriptRuntime;
    public:
        Script(const std::shared_ptr<ScriptRuntime>& runtime, const std::string& source);
        ~Script();

        Int32 Main(); 
    private:
        std::shared_ptr<ScriptRuntime> runtime = nullptr;

        asIScriptModule* module = nullptr;  
    };
}