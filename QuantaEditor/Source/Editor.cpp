#include <iostream>
#include <fstream>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Quanta/Gui/DearImGui/ImGuiRenderer.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <Quanta/CLI/Log/Log.h>
#include <Quanta/CLI/Shell/Shell.h>
#include <Quanta/Scene/Components/TransformComponent.h>
#include <Quanta/Scene/Components/PointLightComponent.h>
#include <Quanta/Scene/Components/ModelRendererComponent.h>
#include <Quanta/Scene/Components/CameraComponent.h>

#include "Editor.h"
#include "Gizmo/ImGuizmo.h"
#include "Widgets/TextEditor/AsLanguageDefinition.h"
#include "Widgets/SceneViewer.h"
#include "Widgets/Style.h"

namespace Quanta
{
    Editor::Editor()
    {
        sceneViewer = std::make_unique<SceneViewer>(scene.GetRegistry(), *window);

        window->SetTitle("Quanta Editor");

        Renderer3D::Create(*window, sceneViewer->GetFrameBuffer());
        ImGuiRenderer::Create(window);

        auto& io = ImGui::GetIO();
        
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Consolas-Font/CONSOLA.TTF", 15.0f);

        auto& style = ImGui::GetStyle();        

        ApplyEditorStyle(style);
        
        const auto skybox = Texture::Create(Texture::Type::CubeMap, TexelFormat::Rgba8I, 1024, 1024, 1);

        std::shared_ptr<Image32> images[] 
        {
            Image32::FromFile("Resources/Textures/Skybox/right.png"),
            Image32::FromFile("Resources/Textures/Skybox/left.png"),
            Image32::FromFile("Resources/Textures/Skybox/top.png"),
            Image32::FromFile("Resources/Textures/Skybox/bottom.png"),
            Image32::FromFile("Resources/Textures/Skybox/back.png"),
            Image32::FromFile("Resources/Textures/Skybox/front.png")
        };

        for (std::size_t i = 0; i < 6; i++)
        {
            const auto& image = *images[i];

            skybox->SetData(image.GetData(), 0, 0, i);
        }

        skyboxSampler = Sampler::Create(skybox);

        Renderer3D::SetEnvironmentSampler(skyboxSampler);
                
        ScriptRuntime::Create();

        std::ifstream source("Resources/Scripts/test.as");
        
        script = std::make_unique<Script>("Resources/Scripts/test.as");

        script->Main();

        textEditor.SetLanguageDefinition(AngelScriptDefinition());

        std::ostringstream strStream;

        strStream << source.rdbuf();

        textEditor.SetText(strStream.str());

        sponza = scene.Create();

        sponza.Set<TransformComponent>();
        sponza.Set<ModelRendererComponent>();

        auto& transform = sponza.Get<TransformComponent>();

        transform.translation.y -= 10;

        auto& modelComponent = sponza.Get<ModelRendererComponent>();

        const auto modelMem = std::make_shared<Model>();
        const auto modelMem2 = std::make_shared<Model>();

        *modelMem = Model::FromFile("Resources/Models/sponza/sponza.fbx"); 

        modelComponent.model = modelMem; 
        
        *modelMem2 = Model::FromFile("Resources/Models/test_scene_01.fbx");

        auto ent2 = scene.Create();

        ent2.Set<TransformComponent>();
        ent2.Set<ModelRendererComponent>();

        auto& transform2 = ent2.Get<TransformComponent>();
        auto& modelComponent2 = ent2.Get<ModelRendererComponent>();
        
        modelComponent2.model = modelMem2;

        auto ent3 = scene.Create();

        ent3.Set<TransformComponent>();
        ent3.Set<ModelRendererComponent>();

        ent3.Get<ModelRendererComponent>() = modelComponent2;

        auto lightEntity = scene.Create();

        lightEntity.Set<TransformComponent>();
        lightEntity.Set<PointLightComponent>();

        auto& lightComp = lightEntity.Get<PointLightComponent>();

        lightComp.ambient = glm::vec3(1.0f);
        lightComp.diffuse = { 0.5f, 0.5f, 0.5f };
        lightComp.specular = { 1.0f, 1.0f, 1.0f };
        
        lightComp.linear = 0.045f;
        lightComp.quadratic = 0.0075f;

        lightEntity.Set<ModelRendererComponent>();

        const auto lightSphere = std::make_shared<Model>();

        *lightSphere = Model::FromFile("Resources/Models/light_sphere.fbx");

        lightEntity.Get<ModelRendererComponent>().model = lightSphere;

        Shell::AddCommand("set_wireframe", Shell::PrimitiveType::Void, { Shell::PrimitiveType::Bool }, []()
        {   
            const auto enable = Shell::GetArgBool(0);

            Renderer3D::EnableWireframe(enable);

            return true;
        });

        Shell::AddCommand("get_wireframe", Shell::PrimitiveType::Bool, { }, []()
        {
            Shell::SetReturnBool(Renderer3D::IsWireframe());
            
            return true;
        });

        Shell::AddCommand("add", Shell::PrimitiveType::Void, { Shell::PrimitiveType::Int, Shell::PrimitiveType::Int }, []()
        {
            const auto res = Shell::GetArgInt(0) + Shell::GetArgInt(1);

            Shell::SetReturnInt(res);

            return true;
        });
                
        Log::Write(Log::Level::Debug, "hello, world!");
    }

    Editor::~Editor()
    {
        ScriptRuntime::Destroy();
        Renderer3D::Destroy();
        ImGuiRenderer::Destroy();
    }

    void Editor::OnUpdate(const float frameTime)
    {
        GraphicsDevice::ClearBackBuffer(glm::vec4(1.0f), 1.0f, 0);

        static auto transform = glm::identity<glm::mat4>();

        ImGuiRenderer::Begin(frameTime);
        {
            ImGuizmo::BeginFrame();

            ImGui::DockSpaceOverViewport();

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit"))
                    {
                        window->Close();
                    }

                    ImGui::EndMenu();   
                }

                if (ImGui::BeginMenu("Window"))
                {   
                    if (ImGui::MenuItem("Text Editor"))
                    {
                        textEditorOpen = !textEditorOpen;
                    }

                    if (ImGui::MenuItem("Log"))
                    {
                        logOpen = !logOpen;
                    }

                    if (ImGui::MenuItem("Scene View"))
                    {
                        sceneViewerOpen = !sceneViewerOpen;
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            log.Render("Log", &logOpen);

            if (textEditorOpen)
            {
                if (ImGui::Begin("Text Editor", &textEditorOpen))
                {
                    static const auto runScript = [](const TextEditor& editor)
                    {
                        std::ofstream file("Resources/Scripts/test.as");
                        
                        const auto source = editor.GetText();

                        file.write(source.c_str(), source.size());

                        file.close();

                        Script script("Resources/Scripts/test.as");

                        script.Main();
                    };
                                   
                    if (ImGui::IsItemFocused() && ImGui::GetIO().KeysDown[static_cast<USize>(Key::F5)])
                    {
                        runScript(textEditor);
                    }

                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::Button("Save"))
                        {
                            std::ofstream file("Resources/Scripts/test.as");

                            const auto text = textEditor.GetText();

                            file.write(text.c_str(), text.size());
                        }

                        if (ImGui::Button("Run"))
                        {
                            runScript(textEditor);
                        }

                        ImGui::EndMenu();
                    }

                    textEditor.Render("Text");
                }

                ImGui::End();
            }
            
            sceneViewer->Render("Scene", &sceneViewerOpen);
            
            if (entityViewOpen)
            {
                if (ImGui::Begin("Entities"))
                {
                    const auto view = scene.GetRegistry().view<TransformComponent>();

                    for (const auto [entity, transform] : view.each())
                    {
                        const auto id = static_cast<std::uint32_t>(entity);

                        ImGui::PushID(id);

                        ImGui::Text("Entity %u", id);   
                        
                        ImGui::DragFloat3("Translation", &transform.translation.x);
                        ImGui::DragFloat3("Scale", &transform.scale.x);
                        ImGui::DragFloat3("Rotation", &transform.rotation.x);

                        ImGui::PopID();
                    } 
                }

                ImGui::End();
            }
        }   
        ImGuiRenderer::End();

        GraphicsDevice::SetRasterPipeline(nullptr);
    }
}