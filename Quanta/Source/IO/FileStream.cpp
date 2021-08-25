#include <Quanta/IO/FileStream.h>

#include "../Debugging/Validation.h"

namespace Quanta
{
    static const char* modes[3] = { "r", "w", "a" };

    FileStream::FileStream(const std::string& path, Mode mode)
    {
        this->path = path;

        handle = std::fopen(path.c_str(), modes[static_cast<std::size_t>(mode)]);

        DEBUG_ASSERT(handle != nullptr);
    }

    FileStream::~FileStream()
    {
        DEBUG_ASSERT(handle != nullptr);

        std::fclose(handle);
    }

    std::string FileStream::ReadAllText() const
    {
        std::string text(GetSize(), '\0');

        std::fread(&text[0], text.size() * sizeof(char), 1, handle);

        return text;
    }
    
    std::vector<UInt8> FileStream::ReadAllBytes() const
    {
        std::vector<UInt8> bytes(GetSize());

        std::fread(bytes.data(), bytes.size(), 1, handle);

        return bytes;
    }

    void FileStream::WriteAllText(const std::string& text)
    {
        DEBUG_ASSERT(mode == Mode::Write);

        std::fwrite(text.data(), text.size(), text.size(), handle);
    }

    void FileStream::WriteAllBytes(const std::vector<UInt8>& bytes)
    {
        DEBUG_ASSERT(mode == Mode::Write);

        std::fwrite(bytes.data(), bytes.size(), bytes.size(), handle);
    }

    std::size_t FileStream::GetSize() const
    {
        std::fseek(handle, 0, SEEK_END);

        std::size_t size = ftell(handle);

        std::fseek(handle, 0, SEEK_SET);

        return size;
    }

    std::string FileStream::GetPath() const
    {  
        return path;
    }

    FileStream::Mode FileStream::GetMode() const
    {
        return mode;
    }
}