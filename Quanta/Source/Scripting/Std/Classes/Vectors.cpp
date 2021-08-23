#include <stdexcept>

#include "Vectors.h"
#include "../TypeDefs/TypeDefs.h"
#include "../../../Utilites/Symbols.h"

namespace Quanta::As_Std
{
    static void Construct(vec2f32& self)
    {
        self.x = 0;
        self.y = 0;
    }
    
    static void Construct(vec2f32& self, f32 value)
    {
        self.x = value;
        self.y = value;
    }
    
    static void Construct(vec2f32& self, f32 x, f32 y)
    {
        self.x = x;
        self.y = y;
    }
    
    static void Construct(vec2f32& self, float* list)
    {
        self.x = list[0];
        self.y = list[1];
    }

    static vec2f32 OpAddAssign(vec2f32& self, vec2f32 other)
    {
        return self = self + other;
    }

    static vec2f32 OpSubAssign(vec2f32& self, vec2f32 other)
    {
        return self = self - other;
    }

    static vec2f32 OpMulAssign(vec2f32& self, vec2f32 other)
    {
        return self = self * other;
    }

    static vec2f32 OpDivAssign(vec2f32& self, vec2f32 other)
    {
        if(other.x == 0.0f || other.y == 0.0f)
        {   
            throw std::runtime_error("division by zero");
        }

        return self = self / other;
    }
    
    static vec2f32 OpAddAssign(vec2f32& self, f32 other)
    {
        return self = self + other;
    }

    static vec2f32 OpSubAssign(vec2f32& self, f32 other)
    {
        return self = self - other;
    }

    static vec2f32 OpMulAssign(vec2f32& self, f32 other)
    {
        return self = self * other;
    }
    
    static vec2f32 OpDivAssign(vec2f32& self, f32 other)
    {
        if(other == 0.0f)
        {   
            throw std::runtime_error("division by zero");
        }

        return self = self / other;
    }

    static f32 OpIndex(const vec2f32& self, usize index)
    {
        if(index > 1) 
        {
            throw std::runtime_error("index was out of range");
        }

        return self[index];
    }

    static f32& OpIndex(vec2f32& self, usize index)
    {
        if(index > 1) 
        {
            throw std::runtime_error("index was out of range");
        }

        return self[index];
    }

    void RegisterVectors(asIScriptEngine* engine)
    {
        engine->RegisterObjectType(nameof(vec2f32), sizeof(vec2f32), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<vec2f32>());
        
        engine->RegisterObjectProperty(nameof(vec2f32), "f32 x", offsetof(vec2f32, vec2f32::x));
        engine->RegisterObjectProperty(nameof(vec2f32), "f32 y", offsetof(vec2f32, vec2f32::y));

        engine->RegisterObjectProperty(nameof(vec2f32), "f32 r", offsetof(vec2f32, vec2f32::r));
        engine->RegisterObjectProperty(nameof(vec2f32), "f32 g", offsetof(vec2f32, vec2f32::g));

        engine->RegisterObjectBehaviour(nameof(vec2f32), asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(Construct, (vec2f32&), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(vec2f32), asBEHAVE_CONSTRUCT, "void f(f32 x)", asFUNCTIONPR(Construct, (vec2f32&, f32), void), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectBehaviour(nameof(vec2f32), asBEHAVE_CONSTRUCT, "void f(f32 x, f32 y)", asFUNCTIONPR(Construct, (vec2f32&, f32, f32), void), asCALL_CDECL_OBJFIRST);
        
        engine->RegisterObjectBehaviour(nameof(vec2f32), asBEHAVE_LIST_CONSTRUCT, "void f(f32& in) { f32, f32 }", asFUNCTIONPR(Construct, (vec2f32&, float*), void), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opAdd(const vec2f32& in) const", asFUNCTIONPR(glm::operator+, (const vec2f32&, const vec2f32&), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opSub(const vec2f32& in) const", asFUNCTIONPR(glm::operator-, (const vec2f32&, const vec2f32&), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opMul(const vec2f32& in) const", asFUNCTIONPR(glm::operator*, (const vec2f32&, const vec2f32&), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opDiv(const vec2f32& in) const", asFUNCTIONPR(glm::operator/, (const vec2f32&, const vec2f32&), vec2f32), asCALL_CDECL_OBJFIRST);
        
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opAdd(f32) const", asFUNCTIONPR(glm::operator+, (const vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opSub(f32) const", asFUNCTIONPR(glm::operator-, (const vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opMul(f32) const", asFUNCTIONPR(glm::operator*, (const vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opDiv(f32) const", asFUNCTIONPR(glm::operator/, (const vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opAddAssign(vec2f32)", asFUNCTIONPR(OpAddAssign, (vec2f32&, vec2f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opSubAssign(vec2f32)", asFUNCTIONPR(OpSubAssign, (vec2f32&, vec2f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opMulAssign(vec2f32)", asFUNCTIONPR(OpMulAssign, (vec2f32&, vec2f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opDivAssign(vec2f32)", asFUNCTIONPR(OpDivAssign, (vec2f32&, vec2f32), vec2f32), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opAddAssign(f32)", asFUNCTIONPR(OpAddAssign, (vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opSubAssign(f32)", asFUNCTIONPR(OpSubAssign, (vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opMulAssign(f32)", asFUNCTIONPR(OpMulAssign, (vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "vec2f32 opDivAssign(f32)", asFUNCTIONPR(OpDivAssign, (vec2f32&, f32), vec2f32), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod(nameof(vec2f32), "f32 opIndex(usize) const", asFUNCTIONPR(OpIndex, (const vec2f32&, usize), f32), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod(nameof(vec2f32), "f32& opIndex(usize)", asFUNCTIONPR(OpIndex, (vec2f32&, usize), f32&), asCALL_CDECL_OBJFIRST);
    }
}