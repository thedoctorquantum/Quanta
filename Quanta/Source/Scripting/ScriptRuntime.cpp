#include <Quanta/Scripting/ScriptRuntime.h>
#include <Quanta/Logging/Log.h>
#include <angelscript.h>
#include <sstream>

#include "../Debugging/Validation.h"
#include "As/Std/Std.h"

namespace Quanta
{
    static void MessageCallback(const asSMessageInfo* const message, void* const param)
    {
        Log::Level level = Log::Level::Information;

        switch(message->type)
        {
            case asMSGTYPE_ERROR:
                level = Log::Level::Error;

                break;
            case asMSGTYPE_WARNING:
                level = Log::Level::Warning;

                break;
            case asMSGTYPE_INFORMATION:
                level = Log::Level::Information;

                break;  
        }
        
        std::ostringstream outputMessage;

        outputMessage << "[Script Compiler] ";
        outputMessage << message->section;
        outputMessage << " (";
        outputMessage << message->row;
        outputMessage << ", ";
        outputMessage << message->col;
        outputMessage << ") : [";
        outputMessage << message->message;
        outputMessage << ']';

        Log::Write(level, outputMessage.str());
    };

    static void ExceptionCallback(asIScriptContext* const context, void* const userParam)
    {
        try 
        {
            throw;
        }
        catch(const std::exception& exception)
        {
            context->SetException(exception.what());

            std::ostringstream message;

            message << "[Script] Exception thrown (Script terminated) (";
            message << context->GetExceptionLineNumber();
            message << ") (";
            message << context->GetExceptionFunction()->GetName();
            message << "): ";
            message << context->GetExceptionString();

            Log::Write(Log::Level::Error, message.str());
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