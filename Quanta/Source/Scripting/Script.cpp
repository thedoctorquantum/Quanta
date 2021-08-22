#include <Quanta/Scripting/Script.h>
#include <Quanta/Scripting/ScriptRuntime.h>

#include "../Debugging/Validation.h"

namespace Quanta
{
    Script::Script(const std::shared_ptr<ScriptRuntime>& runtime, const std::string& source)
    {
        DEBUG_ASSERT(source.size() != 0);

        this->runtime = runtime;

        module = runtime->engine->GetModule(nullptr, asGM_ALWAYS_CREATE);

        module->AddScriptSection("source", source.c_str(), source.size());

        module->Build();
    }

    Script::~Script()
    {
        module->Discard();
    }

    Int32 Script::Main()
    {
        asIScriptFunction* main = module->GetFunctionByDecl("int32 Main()");

        DEBUG_ASSERT(main != nullptr);

        runtime->context->Prepare(main);

        runtime->context->Execute();

        double exit = runtime->context->GetReturnDouble();

        return static_cast<Int32>(exit);
    } 
}