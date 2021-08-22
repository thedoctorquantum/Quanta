#include <cstdio>

#include "Functions.h"

namespace Quanta::As_Std
{
    struct FunctionInfo final 
    {
        const char* signature = nullptr;
        asDWORD callingConvention = 0;

        constexpr FunctionInfo(const char* signature, asDWORD callingConvention)
        {
            this->signature = signature;
            this->callingConvention = callingConvention;
        }
    };

    constexpr FunctionInfo Print_Info { "void Print(const string& in)", asCALL_CDECL };
    constexpr FunctionInfo PutChar_Info { "void PutChar(Char)", asCALL_CDECL };

    void Print(const std::string& string)
    {
        std::printf("%s\n", string.c_str());
    }

    void PutChar(Char character)
    {
        std::putchar(character);
    }

    void RegisterFunctions(asIScriptEngine* engine)
    {
        //engine->RegisterGlobalFunction(Print_Info.signature, asFUNCTION(Print), Print_Info.callingConvention);
        engine->RegisterGlobalFunction(PutChar_Info.signature, asFUNCTION(PutChar), PutChar_Info.callingConvention);
    }
}