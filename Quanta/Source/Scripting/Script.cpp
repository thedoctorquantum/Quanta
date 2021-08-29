#include <Quanta/Scripting/Script.h>
#include <Quanta/Scripting/ScriptRuntime.h>
#include <Quanta/IO/FileStream.h>
#include <Quanta/Text/CPreProcessor.h>
#include <fstream>
#include <ostream>
#include <sstream>

#include "../Debugging/Validation.h"

namespace Quanta
{
    Script::Script(const std::string& filepath)
    {
        module = ScriptRuntime::GetEngine()->GetModule(nullptr, asGM_ALWAYS_CREATE);
        
        CPreProcessor processor(filepath);

        std::string output = processor.Process();

        module->AddScriptSection("Source", output.c_str(), output.size());

        module->Build();
        
        context = ScriptRuntime::GetEngine()->CreateContext();

        DEBUG_ASSERT(context != nullptr);
    }
    
    Script::~Script()
    {
        module->Discard();
        context->Release();
    }
    
    I32 Script::Main()
    {
        asIScriptFunction* main = module->GetFunctionByDecl("int32 Main()");

        if (main == nullptr)
        {
            return -1;
        }

        context->Prepare(main);

        context->Execute();
        
        asQWORD exit = context->GetReturnQWord();

        context->Unprepare();

        return static_cast<I32>(exit);
    } 
}