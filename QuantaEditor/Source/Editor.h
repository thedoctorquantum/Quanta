#pragma once

#include <Quanta/Core/Application.h>

namespace Quanta
{
    class Editor final : public Application
    {
    public:
        Editor();
        ~Editor();

        void OnUpdate(float frameTime) override;
    };
}