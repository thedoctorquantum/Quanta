#include "primitive.h"

namespace Quanta::As::Std
{
    void RegisterPrimitives(asIScriptEngine* engine)
    {
        RegisterIntegers(engine);
        RegisterFloats(engine);
        RegisterVectors(engine);
    }
}