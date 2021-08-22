#include <Quanta/Scripting/ScriptRuntime.h>
#include <cstdio>

#include "../Debugging/Validation.h"
#include "Std/Std.h"

namespace Quanta
{
    static void MessageCallback(const asSMessageInfo* message, void* param)
    {
        const char* type = "";

        switch(message->type)
        {
            case asMSGTYPE_ERROR:
                type = "ERROR";

                break;
            case asMSGTYPE_WARNING:
                type = "WARNING";

                break;
            case asMSGTYPE_INFORMATION:
                type = "INFO";

                break;  
        }

        std::printf("[SCRIPT_COMPILER] %s (%d, %d) : [%s] : [%s]\n", message->section, message->row, message->col, type, message->message);
    };

    ScriptRuntime::ScriptRuntime()
    {
        engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

        DEBUG_ASSERT(engine != nullptr);

        engine->SetEngineProperty(asEP_REQUIRE_ENUM_SCOPE, true);
        engine->SetEngineProperty(asEP_USE_CHARACTER_LITERALS, true);

        context = engine->CreateContext();

        DEBUG_ASSERT(context != nullptr);

        if constexpr (DEBUG)
        {
            engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
        }

        As_Std::Configure(engine);
    }
    
    ScriptRuntime::~ScriptRuntime()
    {
        context->Release();
        engine->Release();
    }
}