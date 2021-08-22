#pragma once

#include <Quanta/Core/Application.h>
#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Scripting/ScriptRuntime.h>
#include <Quanta/Scripting/Script.h>

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

        std::shared_ptr<ScriptRuntime> scriptRuntime = nullptr;
        std::unique_ptr<Script> script = nullptr;
    };
}