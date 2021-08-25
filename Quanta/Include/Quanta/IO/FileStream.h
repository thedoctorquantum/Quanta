#pragma once

#include <cstdio>
#include <vector>
#include <string>

#include "../Math/Integer.h"

namespace Quanta
{
    class FileStream final 
    {
    public:
        enum class Mode : UInt8
        {
            Read,
            Write,
            Append
        };

        FileStream(const std::string& path, Mode mode);
        ~FileStream();

        std::string ReadAllText() const;
        std::vector<UInt8> ReadAllBytes() const;      

        void WriteAllText(const std::string& text);        
        void WriteAllBytes(const std::vector<UInt8>& bytes);   

        std::size_t GetSize() const;

        std::string GetPath() const;
        Mode GetMode() const;
    private:
        std::FILE* handle = nullptr;

        std::string path;
        Mode mode = Mode::Read;
    };
}