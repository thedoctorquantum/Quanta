#include <stdexcept>

#include "Classes.h"
#include "Vectors.h"

namespace Quanta::As_Std
{
    static std::string get_Message(const std::exception& self) 
    {
        return std::string(self.what());
    }

    void RegisterClasses(asIScriptEngine* engine)
    {
        RegisterVectors(engine);

        String::Register(engine);
    }
}