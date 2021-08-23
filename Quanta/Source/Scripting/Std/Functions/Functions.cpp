#include <iostream>
#include <stdexcept>
#include <cstring>

#include "Functions.h"
#include "../../../Debugging/Validation.h"

namespace Quanta::As_Std
{
    static std::string currentException;

    void print(const std::string& string)
    {
        std::cout << string << '\n';
    }
    
    void print(char character)
    {
        std::putchar(character);
    }

    void _throw(const std::string& message)
    {
        currentException = message;

        throw std::runtime_error(message);
    }
    
    std::string get_exception()
    {
        return currentException; 
    }
    
    void RegisterFunctions(asIScriptEngine* engine)
    {
        engine->RegisterGlobalFunction("void print(const String& in)", asFUNCTIONPR(print, (const std::string&), void), asCALL_CDECL);
        engine->RegisterGlobalFunction("void print(char)", asFUNCTIONPR(print, (char), void), asCALL_CDECL);
        engine->RegisterGlobalFunction("void throw(const String& in)", asFUNCTIONPR(_throw, (const std::string&), void), asCALL_CDECL);
        engine->RegisterGlobalFunction("std::String get_exception()", asFUNCTIONPR(get_exception, (void), std::string), asCALL_CDECL);
    }
}