#pragma once

#include <Quanta/Core/Application.h>
#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Scripting/ScriptRuntime.h>
#include <Quanta/Scripting/Script.h>
#include <Quanta/Scene/Entity.h>

#include "Widgets/TextEditor/TextEditor.h"
#include "Widgets/LogWidget.h"

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
        
        std::unique_ptr<Script> script = nullptr;

        TextEditor textEditor;
        bool textEditorOpen = true;

        LogWidget log;
        bool logOpen = true;

        entt::registry scene;
    };
}