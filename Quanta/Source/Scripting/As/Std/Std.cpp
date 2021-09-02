#include <Quanta/CLI/Log/Log.h>

#include "Std.h"
#include "primitive/primitive.h"
#include "String.h"
#include "../../../Debugging/Validation.h"
#include "../../../Utilites/Symbols.h"

namespace Quanta::As::Std
{
    static String exceptionMessage;

    static void Throw(const String& message) 
    {
        exceptionMessage = message;

        throw std::runtime_error(message);
    }

    static String Catch() 
    {
        return exceptionMessage;
    }

    static void LogInfo(const String& message)
    {
        Log::Write(Log::Level::Information, message);
    }

    static void LogWarn(const String& message)
    {
        Log::Write(Log::Level::Warning, message);
    }

    static void LogError(const String& message)
    {
        Log::Write(Log::Level::Error, message);
    }

    void Register(asIScriptEngine* engine)
    {
        DEBUG_ASSERT(engine != nullptr);

        engine->SetDefaultNamespace(nameof(Std));

        RegisterPrimitives(engine);

        RegisterString(engine);

        engine->RegisterGlobalFunction("void Throw(const String& in)", asFUNCTIONPR(Throw, (const String&), void), asCALL_CDECL);
        engine->RegisterGlobalFunction("String Catch()", asFUNCTIONPR(Catch, (void), String), asCALL_CDECL);
        
        engine->RegisterGlobalFunction("void LogInfo(const String& in)", asFUNCTION(LogInfo), asCALL_CDECL);
        engine->RegisterGlobalFunction("void LogWarn(const String& in)", asFUNCTION(LogWarn), asCALL_CDECL);
        engine->RegisterGlobalFunction("void LogError(const String& in)", asFUNCTION(LogError), asCALL_CDECL);

        engine->SetDefaultNamespace("");        
    }
}