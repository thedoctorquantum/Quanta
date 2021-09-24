#include <stdexcept>

#include "vector.h"
#include "float.h"
#include "integer.h"
#include "../../../../Utilites/Symbols.h"

namespace Quanta::As::Std
{
    static void Construct(Vec2F32& self)
    {
        self.x = 0;
        self.y = 0;
    }
    
    static void Construct(Vec2F32& self, const F32 value)
    {
        self.x = value;
        self.y = value;
    }
    
    static void Construct(Vec2F32& self, const F32 x, const F32 y)
    {
        self.x = x;
        self.y = y;
    }
    
    static void Construct(Vec2F32& self, float* const list)
    {
        self.x = list[0];
        self.y = list[1];
    }

    static Vec2F32 OpAddAssign(Vec2F32& self, const Vec2F32 other)
    {
        return self = self + other;
    }

    static Vec2F32 OpSubAssign(Vec2F32& self, const Vec2F32 other)
    {
        return self = self - other;
    }

    static Vec2F32 OpMulAssign(Vec2F32& self, const Vec2F32 other)
    {
        return self = self * other;
    }

    static Vec2F32 OpDivAssign(Vec2F32& self, const Vec2F32 other)
    {
        if(other.x == 0.0f || other.y == 0.0f)
        {   
            throw ::std::runtime_error("division by zero");
        }

        return self = self / other;
    }
    
    static Vec2F32 OpAddAssign(Vec2F32& self, const F32 other)
    {
        return self = self + other;
    }

    static Vec2F32 OpSubAssign(Vec2F32& self, const F32 other)
    {
        return self = self - other;
    }

    static Vec2F32 OpMulAssign(Vec2F32& self, const F32 other)
    {
        return self = self * other;
    }
    
    static Vec2F32 OpDivAssign(Vec2F32& self, const F32 other)
    {
        if(other == 0.0f)
        {   
            throw ::std::runtime_error("division by zero");
        }

        return self = self / other;
    }
    
    static F32 OpIndex(const Vec2F32& self, const USize index)
    {
        if(index > 1) 
        {
            throw std::runtime_error("index was out of range");
        }

        return self[index];
    }

    static F32& OpIndex(Vec2F32& self, const USize index)
    {
        if(index > 1) 
        {
            throw std::runtime_error("index was out of range");
        }

        return self[index];
    }
    
    void RegisterVectors(asIScriptEngine* engine)
    {
        engine->RegisterObjectType(nameof(Vec2F32), sizeof(Vec2F32), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<Vec2F32>());
        
        engine->RegisterObjectProperty(nameof(Vec2F32), "F32 X", offsetof(Vec2F32, Vec2F32::x));
        engine->RegisterObjectProperty(nameof(Vec2F32), "F32 Y", offsetof(Vec2F32, Vec2F32::y));
        
        engine->RegisterObjectProperty(nameof(Vec2F32), "F32 R", offsetof(Vec2F32, Vec2F32::r));
        engine->RegisterObjectProperty(nameof(Vec2F32), "F32 G", offsetof(Vec2F32, Vec2F32::g));
        
        engine->RegisterObjectBehaviour(nameof(Vec2F32), asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(Construct, (Vec2F32&), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(Vec2F32), asBEHAVE_CONSTRUCT, "void f(F32 x)", asFUNCTIONPR(Construct, (Vec2F32&, F32), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(Vec2F32), asBEHAVE_CONSTRUCT, "void f(F32 x, F32 y)", asFUNCTIONPR(Construct, (Vec2F32&, F32, F32), void), asCALL_CDECL_OBJFIRST);
        
        engine->RegisterObjectBehaviour(nameof(Vec2F32), asBEHAVE_LIST_CONSTRUCT, "void f(F32& in) { F32, F32 }", asFUNCTIONPR(Construct, (Vec2F32&, float*), void), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opAdd(const Vec2F32& in) const", asFUNCTIONPR(glm::operator+, (const Vec2F32&, const Vec2F32&), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opSub(const Vec2F32& in) const", asFUNCTIONPR(glm::operator-, (const Vec2F32&, const Vec2F32&), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opMul(const Vec2F32& in) const", asFUNCTIONPR(glm::operator*, (const Vec2F32&, const Vec2F32&), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opDiv(const Vec2F32& in) const", asFUNCTIONPR(glm::operator/, (const Vec2F32&, const Vec2F32&), Vec2F32), asCALL_CDECL_OBJFIRST);
        
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opAdd(F32) const", asFUNCTIONPR(glm::operator+, (const Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opSub(F32) const", asFUNCTIONPR(glm::operator-, (const Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opMul(F32) const", asFUNCTIONPR(glm::operator*, (const Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opDiv(F32) const", asFUNCTIONPR(glm::operator/, (const Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opAddAssign(Vec2F32)", asFUNCTIONPR(OpAddAssign, (Vec2F32&, Vec2F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opSubAssign(Vec2F32)", asFUNCTIONPR(OpSubAssign, (Vec2F32&, Vec2F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opMulAssign(Vec2F32)", asFUNCTIONPR(OpMulAssign, (Vec2F32&, Vec2F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opDivAssign(Vec2F32)", asFUNCTIONPR(OpDivAssign, (Vec2F32&, Vec2F32), Vec2F32), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opAddAssign(F32)", asFUNCTIONPR(OpAddAssign, (Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opSubAssign(F32)", asFUNCTIONPR(OpSubAssign, (Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opMulAssign(F32)", asFUNCTIONPR(OpMulAssign, (Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "Vec2F32 opDivAssign(F32)", asFUNCTIONPR(OpDivAssign, (Vec2F32&, F32), Vec2F32), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(Vec2F32), "F32 opIndex(USize) const", asFUNCTIONPR(OpIndex, (const Vec2F32&, USize), F32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(Vec2F32), "F32& opIndex(USize)", asFUNCTIONPR(OpIndex, (Vec2F32&, USize), F32&), asCALL_CDECL_OBJFIRST);
    }
}