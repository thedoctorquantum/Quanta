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

    void Register(asIScriptEngine* engine)
    {
        DEBUG_ASSERT(engine != nullptr);

        engine->SetDefaultNamespace(nameof(Std));

        RegisterPrimitives(engine);

        RegisterString(engine);

        engine->RegisterGlobalFunction("void Throw(const String& in)", asFUNCTIONPR(Throw, (const String&), void), asCALL_CDECL);
        engine->RegisterGlobalFunction("String Catch()", asFUNCTIONPR(Catch, (void), String), asCALL_CDECL);

        engine->SetDefaultNamespace("");        
    }
}