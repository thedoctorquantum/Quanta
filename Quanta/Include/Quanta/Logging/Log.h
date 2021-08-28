#pragma once

#include <string>
#include <functional>

#include "LevelMask.h"

namespace Quanta::Log
{
    using Callback = std::function<void(Level, const std::string&)>;

    void AddCallback(Callback callback);

    void EnableStdCout(bool enable);

    void SetLevelMask(LevelMask);
    LevelMask GetLevelMask();
    
    void Write(Level level, const std::string& message);
    void WriteFormat(Level level, const std::string& message, ...);
}