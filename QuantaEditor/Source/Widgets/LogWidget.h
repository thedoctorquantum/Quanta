#pragma once

#include <Quanta/Logging/Log.h>

#include <vector>
#include <string>
#include <sstream>

namespace Quanta
{
    class LogWidget final 
    {
    public:
        LogWidget();
        ~LogWidget();

        void Render(const char* title, bool* open = nullptr);
    private:
        struct Message final
        {
            std::string text;
            Log::Level level;
        };

        std::vector<Message> messages;
        
        std::ostringstream stream;

        Log::LevelMask mask = Log::LevelMask::All;
    };
}