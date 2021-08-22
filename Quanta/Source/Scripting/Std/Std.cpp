#include "Std.h"
#include "TypeDefs/TypeDefs.h"
#include "Classes/Classes.h"
#include "Functions/Functions.h"
#include "../../Debugging/Validation.h"

namespace Quanta::As_Std
{
    void Configure(asIScriptEngine* engine)
    {
        DEBUG_ASSERT(engine != nullptr);

        engine->SetDefaultNamespace(ScriptNameSpace);

        RegisterTypeDefs(engine);
        RegisterClasses(engine);
        RegisterFunctions(engine);

        engine->SetDefaultNamespace("");
    }
}