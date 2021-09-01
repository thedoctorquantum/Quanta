#pragma once

#include <Quanta/Core/Application.h>
#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Scripting/ScriptRuntime.h>
#include <Quanta/Scripting/Script.h>
#include <Quanta/Scene/Entity.h>
#include <Quanta/Renderer/Renderer3D.h>

#include "Widgets/TextEditor/TextEditor.h"
#include "Widgets/LogWidget.h"
#include "Widgets/SceneViewer.h"

namespace Quanta
{
    class Editor final : public Application
    {
    public:
        Editor();
        ~Editor();

        void OnUpdate(float frameTime) override;
    private:
        std::unique_ptr<SceneViewer> sceneViewer = nullptr;
        bool sceneViewerOpen = true;

        glm::uvec2 sceneViewPos = glm::uvec2(0);  
        glm::uvec2 sceneViewSize = glm::uvec2(1);  

        std::shared_ptr<Sampler> skyboxSampler = nullptr;
        
        std::unique_ptr<Script> script = nullptr;

        TextEditor textEditor;
        bool textEditorOpen = true;

        Entity sponza;

        LogWidget log;
        bool logOpen = true;

        Scene scene;

        bool entityViewOpen = true; 
    };
}