#pragma once

#include <string_view>
#include <functional>

#include "LevelMask.h"

namespace Quanta::Log
{
    using Callback = std::function<void(Level, const std::string_view&)>;

    void AddCallback(const Callback& callback);

    void EnableStdCout(bool enable);

    void SetLevelMask(LevelMask);
    LevelMask GetLevelMask();
    
    void Write(Level level, const std::string_view& message);
    void WriteFormat(Level level, const std::string_view& message, ...);
}