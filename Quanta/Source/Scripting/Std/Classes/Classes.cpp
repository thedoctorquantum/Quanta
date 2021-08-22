#include "Classes.h"

namespace Quanta::As_Std
{
    void RegisterClasses(asIScriptEngine* engine)
    {
        String::Register(engine);
    }
}