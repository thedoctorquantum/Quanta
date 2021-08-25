#include <cstring>
#include <stdexcept>
#include <unordered_map>

#include "String.h"
#include "../../../Utilites/Symbols.h"
#include "../../../Debugging/Validation.h"

namespace Quanta::As::Std
{
    class StringFactory final : public asIStringFactory
    {
    public:
        using Cache = std::unordered_map<String, int>;

        const void* GetStringConstant(const char* data, asUINT length)
        {
            String str(data, length);

            Cache::iterator it = cache.find(str);
            
            if (it != cache.end())
            {
                it->second++;
            }
            else
            {
                it = cache.insert(Cache::value_type(str, 1)).first;
            }

            return &it->first;
        }

        int ReleaseStringConstant(const void* str)
        {
            const String& string = *static_cast<const String*>(str); 

            Cache::iterator it = cache.find(string);

            int result = asSUCCESS;

            if (it == cache.end())
            {
                result = asERROR;
            }
            else
            {
                it->second--;

                if (it->second == 0)
                {
                    cache.erase(it);
                }
            }

            return result;
        }
        
        int GetRawStringData(const void* str, char* data, asUINT* length) const
        {
            if (str == nullptr)
            {
                return asERROR;
            }

            const String& string = *reinterpret_cast<const String*>(str);

            if (length != nullptr)
            {
                *length = string.size();
            }

            if (data != nullptr)
            {
                std::memcpy(data, string.data(), string.size() * sizeof(Char));
            }

            return asSUCCESS;
        }
    private:
        Cache cache;
    };
    
    static StringFactory factory;
    
    void Construct(String& self)
    {
        new(&self) String;
    }
    
    void Construct(String& self, USize length, Char value)
    {   
        new(&self) String(length, value);
    }

    void Construct(String& self, USize length)
    {   
        Construct(self, length, '\0');
    }

    void ListConstruct(String& self, const Char* data)
    {
        asUINT length = *reinterpret_cast<const asUINT*>(data);

        data += sizeof(asUINT);

        new(&self) String(data, length);
    }

    void Destruct(String& self)
    {
        self.~String();
    }

    bool opEquals(const String& self, const String& other)
    {
        return self == other;
    }

    Char opIndex(const String& self, USize index)
    {
        if (index >= self.size())
        {
            throw std::runtime_error("Out of bounds");
        }

        return self[index];
    }

    Char& opIndex(String& self, USize index)
    {
        if (index >= self.size())
        {
            throw std::runtime_error("Out of bounds");
        }

        return self[index];
    }

    String& opAssign(String& self, const String& other)
    {  
        return self = other;
    }

    String& opAddAssign(String& self, const String& other)
    {
        return self += other;
    }
    
    String& opAddAssign(String& self, Char value)
    {
        return self += value;
    }

    String opAdd(const String& self, const String& other)
    {
        return self + other;
    }

    String opAdd(const String& self, Char value)
    {
        return self + value;
    }

    Bool get_IsEmpty(const String& self)
    {
        return self.empty();
    }
    
    USize get_Length(const String& self)
    {
        return self.size();
    }

    void set_Length(String& self, USize value)
    {
        self.resize(value);
    }

    String ToString(I8 value)
    {
        return std::to_string(static_cast<I32>(value));
    }

    String ToString(I16 value)
    {
        return std::to_string(static_cast<I32>(value));
    }
    
    String ToString(I32 value)
    {
        return std::to_string(value);
    }

    String ToString(I64 value)
    {
        return std::to_string(value);
    }

    String ToString(U8 value)
    {
        return std::to_string(static_cast<U32>(value));
    }

    String ToString(U16 value)
    {
        return std::to_string(static_cast<U32>(value));
    }
    
    String ToString(U32 value)
    {
        return std::to_string(value);
    }

    String ToString(U64 value)
    {
        return std::to_string(value);
    }

    String ToString(F32 value)
    {
        return std::to_string(value);
    }

    String ToString(F64 value)
    {
        return std::to_string(value);
    } 

    String ToString(bool value)
    {
        static String values[2] = { nameof(false), nameof(true) };

        return values[value];
    }
    
    void Print(const String& value)
    {
        std::cout << value << '\n';
    }

    void Print(Char value)
    {
        std::cout << value;
    }

    void RegisterString(asIScriptEngine* engine)
    {
        engine->RegisterObjectType(nameof(String), sizeof(String), asOBJ_VALUE | asGetTypeTraits<String>());

        engine->RegisterStringFactory(nameof(String), &factory);

        engine->RegisterObjectBehaviour(nameof(String), asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(Construct, (String&), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(String), asBEHAVE_CONSTRUCT, "void f(USize, Char)", asFUNCTIONPR(Construct, (String&, USize, Char), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(String), asBEHAVE_CONSTRUCT, "void f(USize) explicit", asFUNCTIONPR(Construct, (String&, USize), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(String), asBEHAVE_DESTRUCT, "void f()", asFUNCTIONPR(Destruct, (String&), void), asCALL_CDECL_OBJFIRST);
        
        engine->RegisterObjectBehaviour(nameof(String), asBEHAVE_LIST_CONSTRUCT, "void f(const Char& in) { repeat Char }", asFUNCTIONPR(ListConstruct, (String&, const Char*), void), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(String), "String& opAssign(const String& in)", asFUNCTIONPR(opAssign, (String&, const String&), String&), asCALL_CDECL_OBJFIRST);        

        engine->RegisterObjectMethod(nameof(String), "String opAdd(const String& in) const", asFUNCTIONPR(opAdd, (const String&, const String&), String), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(String), "String opAdd(Char) const", asFUNCTIONPR(opAdd, (const String&, Char), String), asCALL_CDECL_OBJFIRST);
        
        engine->RegisterObjectMethod(nameof(String), "String& opAddAssign(const String& in)", asFUNCTIONPR(opAddAssign, (String&, const String&), String&), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(String), "String& opAddAssign(Char)", asFUNCTIONPR(opAddAssign, (String&, Char), String&), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(String), "bool opEquals(const String& in) const", asFUNCTIONPR(opEquals, (const String&, const String&), bool), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(String), "Char opIndex(USize) const", asFUNCTIONPR(opIndex, (const String&, USize), Char), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(String), "Char& opIndex(USize)", asFUNCTIONPR(opIndex, (String&, USize), Char&), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(String), "USize get_Length() const property", asFUNCTIONPR(get_Length, (const String&), USize), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(String), "void set_Length(USize) property", asFUNCTIONPR(set_Length, (String&, USize), void), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(String), "bool get_IsEmpty() const property", asFUNCTIONPR(get_IsEmpty, (const String&), Bool), asCALL_CDECL_OBJFIRST);

        engine->RegisterGlobalFunction("String ToString(I8)", asFUNCTIONPR(ToString, (I8), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(I16)", asFUNCTIONPR(ToString, (I16), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(I32)", asFUNCTIONPR(ToString, (I32), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(I64)", asFUNCTIONPR(ToString, (I64), String), asCALL_CDECL);

        engine->RegisterGlobalFunction("String ToString(U8)", asFUNCTIONPR(ToString, (U8), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(U16)", asFUNCTIONPR(ToString, (U16), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(U32)", asFUNCTIONPR(ToString, (U32), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(U64)", asFUNCTIONPR(ToString, (U64), String), asCALL_CDECL);

        engine->RegisterGlobalFunction("String ToString(F32)", asFUNCTIONPR(ToString, (F32), String), asCALL_CDECL);
        engine->RegisterGlobalFunction("String ToString(F64)", asFUNCTIONPR(ToString, (F64), String), asCALL_CDECL);

        engine->RegisterGlobalFunction("String ToString(bool)", asFUNCTIONPR(ToString, (Bool), String), asCALL_CDECL);

        engine->RegisterGlobalFunction("void Print(const String& in)", asFUNCTIONPR(Print, (const String&), void), asCALL_CDECL);
        engine->RegisterGlobalFunction("void Print(Char)", asFUNCTIONPR(Print, (Char), void), asCALL_CDECL);
    }
}