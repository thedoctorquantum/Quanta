#pragma once

#include <string>
#include <angelscript.h>

#include "primitive/primitive.h"

namespace Quanta::As::Std
{
    using String = std::string;

    void Construct(String& self);
    void Construct(String& self, USize length, Char value);
    void Construct(String& self, USize length);

    void ListConstruct(String& self, const Char* data);

    void Destruct(String& self);
    
    bool opEquals(const String& self, const String& other);
    
    Char opIndex(const String& self, USize index);
    Char& opIndex(String& self, USize index);

    String& opAssign(String& self, const String& other);

    String& opAddAssign(String& self, const String& other);
    String& opAddAssign(String& self, Char value);    

    String opAdd(const String& self, const String& other);
    String opAdd(const String& self, Char value);
    
    Bool get_IsEmpty(const String& self);
    
    USize get_Length(const String& self);
    void set_Length(String& self, USize value);

    String ToString(I8 value);
    String ToString(I16 value);
    String ToString(I32 value);
    String ToString(I64 value);
    
    String ToString(U8 value);
    String ToString(U16 value);
    String ToString(U32 value);
    String ToString(U64 value);

    String ToString(F32 value);
    String ToString(F64 value);

    String ToString(bool value);
    
    void Print(const String& value);
    void Print(Char value);

    void RegisterString(asIScriptEngine* engine); 
}