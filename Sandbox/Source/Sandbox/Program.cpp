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

struct Sandbox : public Quanta::Application
{
    Sandbox();
    ~Sandbox();

    void OnUpdate(float frameTime) override;

    std::shared_ptr<Quanta::Texture> skybox;
    std::shared_ptr<Quanta::Sampler> brickSampler;
    std::shared_ptr<Quanta::Sampler> brickNormalSampler;

    Quanta::Model backpack;
    Quanta::Material brickMaterial;

    Quanta::Renderer3D::View view;

    Quanta::Model sponza;
    Quanta::Mesh cube;
    
    glm::vec3 position { 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation { 0.0f, 0.0f, 0.0f };

    std::vector<Quanta::PointLight> lights { 1 };    
};

Sandbox::Sandbox()
{
    window->SetState(Quanta::WindowState::Maximized);
    
    Quanta::ImGuiRenderer::Create(window);
    
    Quanta::Renderer3D::Create(*window, nullptr);

    view.matrix = glm::lookAt(glm::vec3(0.0), glm::vec3(0.0), { 0.0f, 1.0f, 0.0f });
    view.fieldOfView = 60.0f;
    view.far = 10000.0f;
    view.width = 1080;
    view.height = 1917;

    std::ostringstream output;

    output << window->GetFrameBufferSize().x;
    output << ", ";
    output << window->GetFrameBufferSize().y;

    Log::Write(Log::Level::Debug, output.str());

    skybox = Quanta::Texture::Create(Quanta::Texture::Type::CubeMap, Quanta::TexelFormat::Rgba8I, 1024, 1024, 1);

    std::shared_ptr<Quanta::Image32> images[] 
    {
        Quanta::Image32::FromFile("Resources/Textures/Skybox/right.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/left.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/top.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/bottom.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/back.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/front.png")
    };

    for(std::size_t i = 0; i < 6; i++)
    {
        const auto& image = *images[i];

        skybox->SetData(image.GetData(), 0, 0, i);
    }

    std::shared_ptr<Quanta::Texture> brickTexture = Quanta::Texture::Load2D("Resources/Textures/brick_albedo.jpg");

    brickSampler = Quanta::Sampler::Create(brickTexture);

    std::shared_ptr<Quanta::Texture> brickNormalMap = Quanta::Texture::Load2D("Resources/Textures/brick_normal.png");
    
    brickNormalSampler = Quanta::Sampler::Create(brickNormalMap);

    backpack = Quanta::Model::FromFile("Resources/Models/test_scene_01.fbx");

    brickMaterial.albedoSampler = brickSampler;
    brickMaterial.normalSampler = brickNormalSampler;   

    brickMaterial.specular = glm::vec3(0.8f);
    brickMaterial.shininess = 200.0f;
    
    glm::vec3 pos { 0.0f, 0.0f, 0.0f }; 
    glm::vec3 rot { 0.0f, 0.0f, 0.0f };
    
    lights[0].Ambient = glm::vec3(1.0f);
    lights[0].Diffuse = { 0.5f, 0.5f, 0.5f };
    lights[0].Specular = { 1.0f, 1.0f, 1.0f };
    
    lights[0].Position = { 0.0f, 0.0f, 0.0f };
    lights[0].Linear = 0.045f;
    lights[0].Quadratic = 0.0075f;

    sponza = Quanta::Model::FromFile("Resources/Models/sponza/sponza.fbx");
    
    cube = Quanta::Mesh::FromFile("Resources/Models/cube.glb");

    Quanta::Renderer3D::SetEnvironmentSampler(Quanta::Sampler::Create(skybox));
}

Sandbox::~Sandbox()
{
    Quanta::ImGuiRenderer::Destroy();
    Quanta::Renderer3D::Destroy();
}

void Sandbox::OnUpdate(const float frameTime)
{
    Quanta::GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 0.0f }, 1.0f, 0);

    glm::mat4 backpackTransform = glm::mat4(1.0);

    backpackTransform = glm::scale(backpackTransform, glm::vec3(1.0f));
    backpackTransform = glm::translate(backpackTransform, position);
    backpackTransform *= glm::toMat4(glm::quat(rotation));

    Quanta::Renderer3D::BeginPass(view);
    {
        Quanta::Renderer3D::SetPointLights(lights.data(), lights.size());

        Quanta::Renderer3D::DrawModel(backpack, backpackTransform);

        Quanta::Renderer3D::DrawMesh(cube, brickMaterial, glm::mat4(1.0f));
    }
    Quanta::Renderer3D::EndPass();

    Quanta::ImGuiRenderer::Begin(frameTime);
    {
        ImGui::ShowMetricsWindow();

        ImGui::DragFloat("Fov", &view.fieldOfView);

        ImGui::DragFloat3("Pos", &position.x);
        ImGui::DragFloat3("Rot", &rotation.x);

        ImGui::DragFloat("Opacity", &backpack.materials[0].opacity, 0.025f);
        
        ImGui::Spacing();

        for(size_t i = 0; i < lights.size(); i++)
        {
            Quanta::PointLight& light = lights[i];

            ImGui::PushID(i);

            ImGui::Text("Light %u", static_cast<uint32_t>(i));

            ImGui::DragFloat3("Pos", &light.Position.x, 0.5f);
            ImGui::ColorEdit4("Color", &light.Ambient.x, 0.5f); 
            ImGui::DragFloat("Intensity", &light.Intensity, 0.5f);

            ImGui::PopID();
        }

        ImGui::Spacing();
    }
    Quanta::ImGuiRenderer::End();

    Quanta::GraphicsDevice::SetRasterPipeline(nullptr);
}

int main()
{
    Sandbox sandbox;

    sandbox.Run();

    return 0;
}