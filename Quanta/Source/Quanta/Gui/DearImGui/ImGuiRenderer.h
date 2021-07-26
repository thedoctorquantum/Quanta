#pragma once

#include "../../Core/Windowing/Window.h"

namespace Quanta::ImGuiRenderer
{
    void Initialize(Window& window);
    void Shutdown();

    void Begin(float elapsed);
    void End();
}