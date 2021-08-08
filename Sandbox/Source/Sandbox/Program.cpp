#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

#define TO_RADIANS (static_cast<float>(M_PI) / 180.0f) 

class Camera final
{
public:
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 GetView()
    {
        return glm::lookAt(Position, Position + Front, { 0.0f, 1.0f, 0.0f });
    }
};

std::string ReadAllText(const std::string& filepath)
{
    std::stringstream contents;

    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        file.open(filepath);
        {
            contents << file.rdbuf();
        }
        file.close();
    }
    catch(const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
    }
    
    return std::move(contents.str());
}

int main()
{
    std::shared_ptr<Quanta::Window> window = Quanta::Window::Create(Quanta::GraphicsApi::OpenGL);
    
    Quanta::GraphicsDevice::Initialize(window.get());
    
    Quanta::AudioDevice::Initialize(Quanta::AudioApi::OpenAL);

    window->SetState(Quanta::WindowState::Maximized);

    Quanta::ImGuiRenderer::Initialize(*window);
    
    float time = 0;

    Quanta::Renderer3D::Initialize(*window);

    std::shared_ptr<Quanta::Texture> testTexture = Quanta::Texture::Load2D("Resources/Textures/brick_albedo.jpg");

    std::shared_ptr<Quanta::Sampler> brickSampler = Quanta::Sampler::Create(testTexture);

    std::shared_ptr<Quanta::Texture> brickNormalMap = Quanta::Texture::Load2D("Resources/Textures/brick_normal.png");
    
    std::shared_ptr<Quanta::Sampler> brickNormalSampler = Quanta::Sampler::Create(brickNormalMap);

    Quanta::Mesh cube = Quanta::Mesh::FromFile("Resources/Models/cube.fbx");

    Quanta::Material brickMaterial;

    brickMaterial.SetAlbedoSampler(brickSampler.get());
    brickMaterial.SetNormalSampler(brickNormalSampler.get());   
    
    std::shared_ptr<Quanta::Texture> albedo = Quanta::Texture::Load2D("Resources/Models/backpack/diffuse.jpg");
    std::shared_ptr<Quanta::Texture> diffuse = Quanta::Texture::Load2D("Resources/Models/backpack/roughness.jpg");
    std::shared_ptr<Quanta::Texture> specular = Quanta::Texture::Load2D("Resources/Models/backpack/specular.jpg");
    std::shared_ptr<Quanta::Texture> normal = Quanta::Texture::Load2D("Resources/Models/backpack/normal.png");

    std::shared_ptr<Quanta::Sampler> albedoSampler = Quanta::Sampler::Create(albedo);
    std::shared_ptr<Quanta::Sampler> diffuseSampler = Quanta::Sampler::Create(diffuse);
    std::shared_ptr<Quanta::Sampler> specularSampler = Quanta::Sampler::Create(specular);
    std::shared_ptr<Quanta::Sampler> normalSampler = Quanta::Sampler::Create(normal);
        
    albedoSampler->SetMagnification(Quanta::FilterMode::Nearest);
    albedoSampler->SetMinification(Quanta::FilterMode::Nearest);
    
    Quanta::Mesh backpack = Quanta::Mesh::FromFile("Resources/Models/backpack/backpack.obj");
    
    Quanta::Material backpackMaterial;

    backpackMaterial.SetAlbedo({ 1.0f, 1.0f, 1.0f, 1.0f });
    backpackMaterial.SetAlbedoSampler(albedoSampler.get()); 

    backpackMaterial.SetDiffuse({ 0.8f, 0.8f, 0.8f, 1.0f });
    backpackMaterial.SetDiffuseSampler(diffuseSampler.get());

    backpackMaterial.SetSpecular({ 0.5f, 0.5f, 0.5f, 1.0f });
    backpackMaterial.SetSpecularSampler(specularSampler.get());
    
    backpackMaterial.SetNormalSampler(normalSampler.get());

    backpackMaterial.SetShininess(225.0f);

    glm::vec3 pos = { 0.0f, 0.0f, 0.0f }; 
    glm::vec3 rot = { 0.0f, 0.0f, 0.0f };

    Camera camera;

    camera.Position = { 2.5f, 0.0f, 7.5f };
    camera.Front = { 0.0f, 0.0f, -5.0f };
    
    std::vector<Quanta::PointLight> lights(1);
    
    lights[0].Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
    lights[0].Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
    lights[0].Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    lights[0].Position = glm::vec3(0.0f, 0.0f, 1.5f);
    lights[0].Constant = 1.0f;
    lights[0].Linear = 0.007f;
    lights[0].Quadratic = 0.0002f;

    while(window->Exists())
    {
        time += 0.0167f;
        
        window->PollEvents();

        glm::mat4 backpackTransform = glm::mat4(1.0);

        backpackTransform = glm::scale(backpackTransform, glm::vec3(1.0f));
        backpackTransform = glm::translate(backpackTransform, pos);
        backpackTransform *= glm::toMat4(glm::quat(rot));

        rot.y = time;
        
        Quanta::Renderer3D::BeginPass();
        {
            Quanta::Renderer3D::SetView(camera.GetView(), camera.Position);

            Quanta::Renderer3D::SetPointLights(lights.data(), lights.size());
            
            Quanta::Renderer3D::DrawMesh(backpack, backpackMaterial, backpackTransform);
            Quanta::Renderer3D::DrawMesh(cube, brickMaterial, glm::translate(glm::mat4(1.0f), { 5.0f, 0.0f, 0.0f }));
        }
        Quanta::Renderer3D::EndPass();

        Quanta::ImGuiRenderer::Begin(1.0f / 60.0f);
        {
            ImGui::ShowMetricsWindow();

            ImGui::DragFloat3("Pos", &pos.x, 0.025f);
            ImGui::DragFloat3("Rot", &rot.x, 0.025f);

            ImGui::Spacing();

            for(size_t i = 0; i < lights.size(); i++)
            {
                Quanta::PointLight& light = lights[i];

                ImGui::PushID(i);

                ImGui::Text("Light %u", static_cast<uint32_t>(i));

                ImGui::DragFloat3("Pos", &light.Position.x, 0.025f);

                ImGui::PopID();
            }

            ImGui::Spacing();

            ImGui::DragFloat3("CamPos", &camera.Position.x, 0.025f);
            ImGui::DragFloat3("CamRot", &camera.Rotation.x, 0.025f);

            ImGui::Begin("Sampler2D");
            {
                if(ImGui::Button("Toggle Filter Mode"))
                {
                    if(albedoSampler->GetMagnification() == Quanta::FilterMode::Linear)
                    {
                        albedoSampler->SetMagnification(Quanta::FilterMode::Nearest);
                        albedoSampler->SetMinification(Quanta::FilterMode::Nearest);
                    }
                    else
                    {
                        albedoSampler->SetMagnification(Quanta::FilterMode::Linear);
                        albedoSampler->SetMinification(Quanta::FilterMode::Linear);
                    }
                }
            }
            ImGui::End();
        }
        Quanta::ImGuiRenderer::End();

        window->SwapBuffers();

        Quanta::GraphicsDevice::SetRasterPipeline(nullptr);

        Quanta::GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 0.0f }, 1.0f, 0);
    }
    
    Quanta::Renderer3D::DeInitialize();

    Quanta::ImGuiRenderer::Shutdown();

    Quanta::GraphicsDevice::DeInitialize();
    Quanta::AudioDevice::DeInitialize();

    return 0;
}