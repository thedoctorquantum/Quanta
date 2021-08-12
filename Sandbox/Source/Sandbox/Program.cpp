#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

int main()
{
    std::shared_ptr<Quanta::Window> window = Quanta::Window::Create(Quanta::GraphicsApi::OpenGL);
    
    Quanta::GraphicsDevice::Initialize(window.get());
    
    Quanta::AudioDevice::Initialize(Quanta::AudioApi::OpenAL);

    window->SetState(Quanta::WindowState::Maximized);

    Quanta::ImGuiRenderer::Initialize(*window);
    
    float time = 0;

    Quanta::Renderer3D::Create(*window);

    std::shared_ptr<Quanta::Texture> skybox = Quanta::Texture::Create(Quanta::TextureType::CubeMap, Quanta::TexelFormat::Rgba8I, 1024, 1024, 1);

    std::shared_ptr<Quanta::Image32> images[] 
    {
        Quanta::Image32::FromFile("Resources/Textures/Skybox/right.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/left.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/top.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/bottom.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/back.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/front.png")
    };

    for(size_t i = 0; i < 6; i++)
    {
        Quanta::Image32& image = *images[i];

        skybox->SetData(image.GetData(), 0, 0, i);
    }

    std::shared_ptr<Quanta::Texture> testTexture = Quanta::Texture::Load2D("Resources/Textures/brick_albedo.jpg");

    std::shared_ptr<Quanta::Sampler> brickSampler = Quanta::Sampler::Create(testTexture);

    std::shared_ptr<Quanta::Texture> brickNormalMap = Quanta::Texture::Load2D("Resources/Textures/brick_normal.png");
    
    std::shared_ptr<Quanta::Sampler> brickNormalSampler = Quanta::Sampler::Create(brickNormalMap);

    Quanta::Model backpack = Quanta::Model::FromFile("Resources/Models/test_scene_01.fbx");
    
    Quanta::Material brickMaterial;

    brickMaterial.SetAlbedoSampler(brickSampler);
    brickMaterial.SetNormalSampler(brickNormalSampler);   

    brickMaterial.SetSpecular(glm::vec3(0.8f));
    brickMaterial.SetShininess(200.0f);
    
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f }; 
    glm::vec3 rot = { 0.0f, 0.0f, 0.0f };

    std::vector<Quanta::PointLight> lights(1);
    
    lights[0].Ambient = glm::vec3(1.0f);
    lights[0].Diffuse = { 0.5f, 0.5f, 0.5f };
    lights[0].Specular = { 1.0f, 1.0f, 1.0f };
    
    lights[0].Position = glm::vec3(0.0f, 0.0f, 0.0f);
    lights[0].Linear = 0.045f;
    lights[0].Quadratic = 0.0075f;
    lights[0].Intensity = 1.0f;

    Quanta::Model model = Quanta::Model::FromFile("Resources/Models/sponza/sponza.fbx");
    
    Quanta::Mesh cube = Quanta::Mesh::FromFile("Resources/Models/cube.glb");

    Quanta::DirectionalLight sun;

    sun.Direction = { -0.2f, -1.0f, -0.3f };
    
    sun.Ambient = glm::vec3(0.5f);
    sun.Diffuse = glm::vec3(0.4f);
    sun.Specular = glm::vec3(0.5f);
    
    Quanta::Renderer3D::SetEnvironmentSampler(Quanta::Sampler::Create(skybox));

    Quanta::Renderer3D::View view;

    view.far = 10000.0f;
    view.fieldOfView = 60.0f;
    view.position = { 0.0f, 0.0f, 0.0f };

    while(window->Exists())
    {
        time += 0.0167f;
        
        window->PollEvents();

        glm::mat4 backpackTransform = glm::mat4(1.0);

        backpackTransform = glm::scale(backpackTransform, glm::vec3(1.0f));
        backpackTransform = glm::translate(backpackTransform, pos);
        backpackTransform *= glm::toMat4(glm::quat(rot));

        //rot.y = time;

        Quanta::Renderer3D::BeginPass(view);
        {
            Quanta::Renderer3D::SetDirectionalLight(sun);
            Quanta::Renderer3D::SetPointLights(lights.data(), lights.size());
            
            Quanta::Renderer3D::DrawModel(model, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

            Quanta::Renderer3D::DrawModel(backpack, backpackTransform);

            Quanta::Renderer3D::DrawMesh(cube, brickMaterial, glm::mat4(1.0f));
        }
        Quanta::Renderer3D::EndPass();

        Quanta::ImGuiRenderer::Begin(1.0f / 60.0f);
        {
            ImGui::ShowMetricsWindow();

            ImGui::DragFloat("Fov", &view.fieldOfView);

            ImGui::DragFloat3("Pos", &pos.x);
            ImGui::DragFloat3("Rot", &rot.x);

            float opacity = backpack.GetMaterials()[0].GetOpacity();

            ImGui::DragFloat("Opacity", &opacity, 0.025f);

            backpack.GetMaterials()[0].SetOpacity(opacity);

            ImGui::Spacing();

            ImGui::DragFloat3("Sun Direction", &sun.Direction.x, 0.125f, -1.0f, 1.0f);

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

            ImGui::DragFloat3("CamPos", &view.position.x, 0.125f);
            ImGui::DragFloat3("CamRot", &view.rotation.x, 0.125f);
        }
        Quanta::ImGuiRenderer::End();

        window->SwapBuffers();

        Quanta::GraphicsDevice::SetRasterPipeline(nullptr);

        Quanta::GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 0.0f }, 1.0f, 0);
    }
    
    Quanta::Renderer3D::Destroy();

    Quanta::ImGuiRenderer::Shutdown();

    Quanta::GraphicsDevice::DeInitialize();
    Quanta::AudioDevice::DeInitialize();

    return 0;
}