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
        std::shared_ptr<FrameBuffer> sceneBuffer;
        glm::uvec2 sceneViewPos = glm::uvec2(0);  
        glm::uvec2 sceneViewSize = glm::uvec2(1);  

        std::shared_ptr<Sampler> sceneSampler = nullptr;

        std::shared_ptr<Sampler> skyboxSampler = nullptr;

        Renderer3D::View view;

        Model model;
        
        std::unique_ptr<Script> script = nullptr;

        TextEditor textEditor;
        bool textEditorOpen = true;

        LogWidget log;
        bool logOpen = true;

        bool sceneViewOpen = true;

        entt::registry scene;
    };
}