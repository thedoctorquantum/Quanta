#include <Quanta/IO/FileStream.h>

#include "../Debugging/Validation.h"

namespace Quanta
{
    static const char* modes[] = { "rb", "wb", "rw", "a" };

    FileStream::FileStream(const std::string& path, const Mode mode)
    {
        this->path = path;
        this->mode = mode;

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
    
    std::vector<U8> FileStream::ReadAllBytes() const
    {
        std::vector<U8> bytes(GetSize());

        std::fread(bytes.data(), bytes.size(), 1, handle);

        return bytes;
    }

    void FileStream::WriteAllText(const std::string& text)
    {
        DEBUG_ASSERT(mode == Mode::Write || mode == Mode::ReadWrite);

        std::fwrite(text.data(), text.size(), 1, handle);
    }

    void FileStream::WriteAllBytes(const std::vector<U8>& bytes)
    {
        DEBUG_ASSERT(mode == Mode::Write || mode == Mode::ReadWrite);

        std::fwrite(bytes.data(), bytes.size(), 1, handle);
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