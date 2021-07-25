#pragma once

#include "../../Core/Window.h"

namespace Quanta::ImGuiRenderer
{
    void Initialize(const Window& window);
    void Shutdown();

    void Begin(float elapsed);
    void End();
}