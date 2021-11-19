#pragma once

#include <Quanta/CLI/Log/Log.h>
#include <vector>
#include <string>

namespace Quanta
{
    struct LogWidget 
    {
        struct Message final
        {
            std::string text;
            Log::Level level;
        };

        std::vector<Message> messages;
        
        Log::LevelMask mask = Log::LevelMask::All;

        char input[100] { }; 

        bool open = true;

        LogWidget();

        void Render(const char* title);
    };
}