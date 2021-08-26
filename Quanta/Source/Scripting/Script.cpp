#include <Quanta/Scripting/Script.h>
#include <Quanta/Scripting/ScriptRuntime.h>

#include "../Debugging/Validation.h"

namespace Quanta
{
    Script::Script(const std::string& source)
    {
        DEBUG_ASSERT(source.size() != 0);

        module = ScriptRuntime::GetEngine()->GetModule(nullptr, asGM_ALWAYS_CREATE);
        
        module->AddScriptSection("source", source.c_str(), source.size());

        module->Build();

        context = ScriptRuntime::GetEngine()->CreateContext();

        DEBUG_ASSERT(context != nullptr);
    }
    
    Script::~Script()
    {
        module->Discard();
    }
    
    I32 Script::Main()
    {
        asIScriptFunction* main = module->GetFunctionByDecl("int32 Main()");

        DEBUG_ASSERT(main != nullptr);

        context->Prepare(main);

        context->Execute();
        
        double exit = context->GetReturnDouble();

        context->Unprepare();

        return static_cast<I32>(exit);
    } 
}