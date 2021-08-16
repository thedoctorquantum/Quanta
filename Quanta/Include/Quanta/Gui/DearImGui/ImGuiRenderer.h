#pragma once

#include "../../Core/Windowing/Window.h"

namespace Quanta::ImGuiRenderer
{
    void Create(const std::shared_ptr<Window>& window);
    void Destroy();

    void Begin(float elapsed);
    void End();
}