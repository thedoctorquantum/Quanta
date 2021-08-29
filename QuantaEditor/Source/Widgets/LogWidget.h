#pragma once

#include <Quanta/Logging/Log.h>

#include <vector>
#include <string>

namespace Quanta
{
    class LogWidget final 
    {
    public:
        LogWidget();

        void Render(const char* title, bool* open = nullptr);
    private:
        struct Message final
        {
            std::string text;
            Log::Level level;
        };

        std::vector<Message> messages;
        
        Log::LevelMask mask = Log::LevelMask::All;
    };
}