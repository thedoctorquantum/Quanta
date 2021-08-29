#include <Quanta/Logging/Log.h>
#include <vector>
#include <iostream>
#include <cstdarg>

#include "../Debugging/Validation.h"

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
            case Level::Warning:
                return "Warning";
            case Level::Error:
                return "Error";
            default:
                return "Information";
        }
    }

    struct 
    {
        std::vector<Callback> callbacks;
        bool writeToCout = false;
        LevelMask levelMask = LevelMask::All;
    } static log;

    void AddCallback(Callback callback)
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
    
    void WriteFormat(const Level level, const std::string& message, ...)
    {
        va_list args;
        
        va_start(args, message);

        std::string formatted(200, '\0');

        snprintf(&formatted[0], formatted.size(), message.c_str(), args);

        va_end(args);

        Log::Write(level, formatted);
    }

    void Write(const Level level, const std::string& message)
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
                case Level::Warning:
                    begin = "\x1B[33m";
                    end = "\033[0m\t\t\n";

                    break;
                case Level::Error:
                    begin = "\x1B[31m";
                    end = "\033[0m\t\t\n";

                    break;
            }

            DEBUG_ASSERT(begin != nullptr);
            DEBUG_ASSERT(end != nullptr);

            std::cout << begin << "[Log] " << '[' << LevelToString(level) << "]: " << message.c_str() << end;
        }
        
        for (std::size_t i = 0; i < log.callbacks.size(); i++)
        {
            Callback callback = log.callbacks[i];

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