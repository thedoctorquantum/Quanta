#include <Quanta/CLI/Log/Log.h>
#include <vector>
#include <cstdarg>
#include <cstdio>

#include "../../Debugging/Validation.h"

namespace Quanta::Log
{
    static const char* LevelToString(const Level level)
    {
        switch (level)
        {
            case Level::Trace:
                return "Trace";
            case Level::Information:
                return "Information";
            case Level::Debug:
                return "Debug";
            case Level::Warning:
                return "Warning";
            case Level::Error:
                return "Error";
            case Level::Fatal:
                return "Fatal";
        }

        return nullptr;
    }
    
    struct 
    {
        std::vector<Callback> callbacks;
        bool writeToCout = true;
        LevelMask levelMask = LevelMask::All;
    } static log;

    void AddCallback(const Callback& callback)
    {
        log.callbacks.push_back(callback);
    }

    void EnableStdCout(const bool enable)
    {
        log.writeToCout = enable;
    }
    
    void SetLevelMask(const LevelMask value)
    {
        log.levelMask = value;
    }

    LevelMask GetLevelMask()
    {
        return log.levelMask;
    }
    
    void WriteFormat(const Level level, const std::string_view& message, ...)
    {
        va_list args { };
        
        va_start(args, message);

        const auto size = std::vsnprintf(nullptr, 0, message.data(), args);

        std::string output(size + 1, '\0');
        
        std::vsnprintf(&output[0], size, message.data(), args);

        Log::Write(level, output);

        va_end(args);
    }
    
    void Write(const Level level, const std::string_view& message)
    {
        if (!(log.levelMask & level))
        {
            return;
        }
        
        if (log.writeToCout)
        {   
            const char* begin = nullptr;
            const char* end = nullptr;

            switch (level)
            {
                case Level::Trace:
                    begin = "";
                    end = "\n";

                    break;
                case Level::Information:
                    begin = "\x1B[32m";
                    end = "\033[0m\t\t\n";

                    break;
                case Level::Debug:
                    begin = "\x1B[32m";
                    end = "\033[0m\t\t\n";

                    break;
                case Level::Warning:
                    begin = "\x1B[33m";
                    end = "\033[0m\t\t\n";

                    break;
                case Level::Error:
                    begin = "\x1B[31m";
                    end = "\033[0m\t\t\n";

                    break;
                case Level::Fatal:
                    begin = "\x1B[31m";
                    end = "\033[0m\t\t\n";

                    break;
            }

            DEBUG_ASSERT(begin != nullptr);
            DEBUG_ASSERT(end != nullptr);

            std::fprintf(stderr, "%s[Log] [%s]: %s%s", begin, LevelToString(level), message.data(), end);
        }
        
        for (std::size_t i = 0; i < log.callbacks.size(); i++)
        {
            const auto& callback = log.callbacks[i];

            if (!callback) 
            {
                log.callbacks.erase(log.callbacks.begin() + i);

                i--;

                continue;
            }

            callback(level, message);
        }
    }
}