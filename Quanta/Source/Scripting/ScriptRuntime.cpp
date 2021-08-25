#include <Quanta/Scripting/ScriptRuntime.h>
#include <angelscript.h>
#include <cstdio>

#include "../Debugging/Validation.h"
#include "As/Std/Std.h"

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

    static void ExceptionCallback(asIScriptContext* context, void* userParam)
    {
        try 
        {
            throw;
        }
        catch(const std::exception& exception)
        {
            std::printf("[SCRIPT] [EXCEPTION_THROWN] (Script terminated): %s\n", exception.what());

            context->SetException(exception.what());
        }
    }
    
    static asIScriptEngine* engine = nullptr;

    void ScriptRuntime::Create()
    {
        engine = asCreateScriptEngine();

        DEBUG_ASSERT(engine != nullptr);

        engine->SetEngineProperty(asEP_REQUIRE_ENUM_SCOPE, true);
        engine->SetEngineProperty(asEP_USE_CHARACTER_LITERALS, true);
        engine->SetEngineProperty(asEP_SCRIPT_SCANNER, 0);

        if constexpr (DEBUG)
        {
            engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
        }
        
        engine->SetTranslateAppExceptionCallback(asFUNCTION(ExceptionCallback), nullptr, asCALL_CDECL);
        
        As::Std::Register(engine);
    }
    
    void ScriptRuntime::Destroy()
    {
        engine->Release();
    }
    
    asIScriptEngine* ScriptRuntime::GetEngine() 
    {
        return engine;
    }
}