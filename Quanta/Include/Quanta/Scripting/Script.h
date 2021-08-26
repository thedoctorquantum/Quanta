#pragma once

#include <memory>
#include <angelscript.h>

#include "../Math/Integer.h"

namespace Quanta
{
    class Script final
    {
    public:
        Script(const std::string& source);
        ~Script();

        I32 Main(); 
    private:
        asIScriptModule* module = nullptr;  
        asIScriptContext* context = nullptr;
    };
}