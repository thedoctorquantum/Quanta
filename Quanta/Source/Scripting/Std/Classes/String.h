#pragma once

#include <Quanta/Math/Integer.h>
#include <string>
#include <angelscript.h>

namespace Quanta::As_Std
{
    class CString final
    {
    public:
        static void Register(asIScriptEngine* engine);

        CString();
        CString(Size length);

        CString(const char* string);
        CString(const std::string& string);

        ~CString();

        char& operator[](Size index);
        char operator[](Size index) const;

        void Resize(Size length);

        char* GetData() const;
        Size GetLength() const;
    private:
        std::vector<char> data;
    };
}

namespace Quanta::As_Std::String
{
    void Register(asIScriptEngine* engine);    
}