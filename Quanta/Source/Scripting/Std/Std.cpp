#include "Std.h"
#include "TypeDefs.h"
#include "Functions.h"
#include "../../Debugging/Validation.h"

namespace Quanta::As_Std
{
    void Configure(asIScriptEngine* engine)
    {
        DEBUG_ASSERT(engine != nullptr);

        engine->SetDefaultNamespace(ScriptNameSpace);

        RegisterTypeDefs(engine);
        RegisterFunctions(engine);
    }
}