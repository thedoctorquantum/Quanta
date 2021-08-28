#include <Quanta/Gui/DearImGui/ImGuiRenderer.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <Quanta/Logging/Log.h>
#include <iostream>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "Editor.h"
#include "Gizmo/ImGuizmo.h"
#include "Widgets/TextEditor/AsLanguageDefinition.h"

namespace Quanta
{
    Editor::Editor()
    {
        window->SetTitle("Quanta Editor");
        window->SetState(WindowState::Maximized);

        Renderer3D::Create(*window);
        ImGuiRenderer::Create(window);

        ImGuiIO& io = ImGui::GetIO();
        
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Consolas-Font/CONSOLA.TTF", 15.0f);

        ImGuiStyle& style = ImGui::GetStyle();        

        style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        style.GrabRounding = style.FrameRounding = 2.3f;
        
        model = Model::FromFile("Resources/Models/test_scene_01.fbx");

        Log::EnableStdCout(true);

        Log::SetLevelMask(Log::LevelMask::All);
        
        Log::Write(Log::Level::Information, "Hello, log!");
        Log::Write(Log::Level::Trace, "Trace");
        Log::Write(Log::Level::Warning, "Warning example"); 
        Log::Write(Log::Level::Error, "Error example");

        Log::WriteFormat(Log::Level::Warning, "Hello, world! look at this ptr: %p", &model);

        view.fieldOfView = 70.0f;
        view.far = 10000.0f;
        view.matrix = glm::mat4(1.0f);

        ScriptRuntime::Create();

        std::ifstream source("Resources/Scripts/test.as");
        
        script = std::make_unique<Script>("Resources/Scripts/test.as");

        script->Main();

        textEditor.SetLanguageDefinition(AngelScriptDefinition());

        std::ostringstream strStream;

        strStream << source.rdbuf();

        textEditor.SetText(strStream.str());
    }

    Editor::~Editor()
    {
        ScriptRuntime::Destroy();
        Renderer3D::Destroy();
        ImGuiRenderer::Destroy();
    }
    
    void Editor::OnUpdate(float frameTime)
    {
        GraphicsDevice::ClearBackBuffer(glm::vec4(1.0f), 1.0f, 0);

        static DirectionalLight light;

        static glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });

        Renderer3D::BeginPass(view);
        {
            //Renderer3D::SetDirectionalLight(light);

            //Renderer3D::DrawModel(model, transform);
        }
        Renderer3D::EndPass();

        ImGuiRenderer::Begin(frameTime);
        {
            ImGuizmo::BeginFrame();

            ImGui::DockSpaceOverViewport();

            if (ImGui::BeginMainMenuBar());
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit", "Alt+F4"))
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

                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            log.Render("Log", &logOpen);

            if (textEditorOpen)
            {
                if (ImGui::Begin("Text Editor", &textEditorOpen))
                {
                    static auto runScript = [](const TextEditor& editor)
                    {
                        std::ofstream file("Resources/Scripts/test.as");
                        
                        std::string source = editor.GetText();

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

                            std::string text = textEditor.GetText();

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

            ImGuizmo::SetRect(0, 0, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()));
            ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
            ImGuizmo::AllowAxisFlip(false);

            ImGuizmo::ViewManipulate(glm::value_ptr(view.matrix), 100, ImVec2 { 0, 0 }, ImVec2 { 128, 128 }, 0x00FFFFFF);

            glm::mat4 proj = Renderer3D::GetProjectionMatrix();
            
            ImGuizmo::Enable(true);
            
            ImGuizmo::Manipulate(glm::value_ptr(view.matrix), glm::value_ptr(proj), ImGuizmo::OPERATION::UNIVERSAL, ImGuizmo::MODE::WORLD, glm::value_ptr(transform));
        }   
        ImGuiRenderer::End();

        GraphicsDevice::SetRasterPipeline(nullptr);
    }
}