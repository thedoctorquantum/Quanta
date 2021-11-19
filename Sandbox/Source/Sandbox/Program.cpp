#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Core/Application.h>
#include <Quanta/Gui/DearImGui/ImGuiRenderer.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <Quanta/Memory/Span.h>
#include <Quanta/Memory/Pointer.h>
#include <Quanta/CLI/Log/Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

using namespace Quanta;

struct Sandbox : public Application
{
    Sandbox();
    ~Sandbox();

    void OnUpdate(float frameTime) override;
};

Sandbox::Sandbox()
{
    window->SetState(WindowState::Maximized);
    
    ImGuiRenderer::Create(window);
}

Sandbox::~Sandbox()
{
    ImGuiRenderer::Destroy();
}

void Sandbox::OnUpdate(const float frameTime)
{
    GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 0.0f }, 1.0f, 0);

    ImGuiRenderer::Begin(frameTime);
    {
        ImGui::ShowDemoWindow();
    }
    ImGuiRenderer::End();

    GraphicsDevice::SetRasterPipeline(nullptr);
}

int main()
{
    Sandbox sandbox;

    sandbox.Run();

    return 0;
}