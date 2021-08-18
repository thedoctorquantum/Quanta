#pragma once

#include <Quanta/Core/Application.h>
#include <Quanta/Renderer/Renderer3D.h>

namespace Quanta
{
    class Editor final : public Application
    {
    public:
        Editor();
        ~Editor();

        void OnUpdate(float frameTime) override;
    private:
        Renderer3D::View view;

        Model model;
    };
}