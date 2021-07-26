#pragma once

#include "../../Core/Window.h"

namespace Quanta::ImGuiRenderer
{
    void Initialize(Window& window);
    void Shutdown();

    void Begin(float elapsed);
    void End();
}