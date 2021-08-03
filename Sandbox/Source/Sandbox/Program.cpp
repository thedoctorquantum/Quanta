#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

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

int main(int argc, char** argv)
{
    std::shared_ptr<Quanta::Window> window = Quanta::Window::Create();
    
    Quanta::GraphicsDevice::Initialize(Quanta::GraphicsApi::OpenGL);
    Quanta::AudioDevice::Initialize(Quanta::AudioApi::OpenAL);

    window->SetState(Quanta::WindowState::Maximized);

    float vertices[3 * 9] = 
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f
    };
    
    uint8_t indices[3] =
    {
        0, 1, 2
    };

    std::shared_ptr<Quanta::VertexArray> vertexArray = Quanta::VertexArray::Create();

    std::shared_ptr<Quanta::GraphicsBuffer> vertexBuffer = Quanta::GraphicsBuffer::Create(Quanta::BufferUsage::Static, sizeof(vertices));
    std::shared_ptr<Quanta::GraphicsBuffer> indexBuffer = Quanta::GraphicsBuffer::Create(Quanta::BufferUsage::Static, sizeof(indices));
    
    vertexBuffer->SetData(vertices, sizeof(vertices));
    indexBuffer->SetData(indices, sizeof(indices));
    
    Quanta::VertexLayout layout;

    layout.Add({
        Quanta::BufferPrimitive::Float,
        3,
        sizeof(float),
        false
    });
    
    layout.Add({
        Quanta::BufferPrimitive::Float,
        4,
        sizeof(float),
        false
    });
    
    layout.Add({
        Quanta::BufferPrimitive::Float,
        2,
        sizeof(float),
        false
    });
    
    vertexArray->SetVertexBuffer(vertexBuffer, layout);
    vertexArray->SetIndexBuffer(indexBuffer, Quanta::IndexType::UInt8);
    
    std::string vertexCode = ReadAllText("Resources/Shaders/vertex.glsl");
    std::string fragmentCode = ReadAllText("Resources/Shaders/fragment.glsl");
    
    Quanta::RasterPipelineDescription desc;
    
    desc.ShaderModules.emplace_back(Quanta::ShaderModule::Create(Quanta::ShaderType::Vertex, vertexCode));
    desc.ShaderModules.emplace_back(Quanta::ShaderModule::Create(Quanta::ShaderType::Pixel, fragmentCode));
    
    std::shared_ptr<Quanta::GraphicsBuffer> uniforms = Quanta::GraphicsBuffer::Create(Quanta::BufferUsage::Static, sizeof(glm::mat4) * 2);

    desc.UniformBuffers.emplace_back(uniforms);
    
    std::shared_ptr<Quanta::RasterPipeline> pipeline = Quanta::RasterPipeline::Create(desc);
    
    pipeline->SetPolygonFillMode(Quanta::PolygonFillMode::Solid);   
    pipeline->SetFaceCullMode(Quanta::FaceCullMode::Back);
    pipeline->SetDepthTestMode(Quanta::DepthTestMode::None);
    pipeline->SetEnableDepthWriting(true);
    pipeline->SetBlendMode(Quanta::BlendMode::Add);     

    glm::vec3 translation = glm::vec3(0.0f);

    Quanta::ImGuiRenderer::Initialize(*window);
    
    std::shared_ptr<Quanta::Texture2D> texture = Quanta::Texture2D::FromFile("Resources/Textures/tileset.png");
    
    std::shared_ptr<Quanta::Sampler2D> sampler = Quanta::Sampler2D::Create(texture);
        
    sampler->SetMagnification(Quanta::FilterMode::Linear);
    sampler->SetMinification(Quanta::FilterMode::Linear);

    std::vector<std::shared_ptr<Quanta::Image32>> images =
    {
        Quanta::Image32::FromFile("Resources/Textures/Skybox/right.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/left.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/top.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/bottom.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/back.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/front.png")
    };
    
    std::shared_ptr<Quanta::CubeMap> cubeMap = Quanta::CubeMap::FromImages(images);

    std::shared_ptr<Quanta::SamplerCube> cubeSampler = Quanta::SamplerCube::Create(cubeMap);

    cubeSampler->SetIsSeamless(true);

    float time = 0;
    
    while(window->Exists())
    {
        time += 0.0167f;
        
        window->PollEvents();

        pipeline->SetViewport({ 0.0f, 0.0f, window->GetWidth(), window->GetHeight() });

        glm::mat4 proj = glm::ortho(0.0f, (float) window->GetWidth(), 0.0f, (float) window->GetHeight(), 0.1f, 100.0f);

        uniforms->SetData(&proj, sizeof(glm::mat4), sizeof(glm::mat4));

        glm::mat4 model = glm::mat4(1.0f);

        translation.x = sin(time) * 0.1f;

        model = glm::translate(model, translation);
        
        uniforms->SetData(&model, sizeof(glm::mat4));
        
        Quanta::GraphicsDevice::SetRasterPipeline(pipeline);
        Quanta::GraphicsDevice::SetVertexArray(vertexArray);

        Quanta::GraphicsDevice::BindSampler(*sampler, 0);
        Quanta::GraphicsDevice::BindSampler(*cubeSampler, 0);

        Quanta::DrawCommand cmd;
        
        cmd.Count = 3;
        
        Quanta::GraphicsDevice::DispatchDraw(cmd);

        Quanta::ImGuiRenderer::Begin(1.0f / 60.0f);
        {
            ImGui::ShowDemoWindow();
            ImGui::ShowMetricsWindow();

            ImGui::DragFloat3("Pos", &translation.x, 0.025f);

            ImGui::Begin("Sampler2D");
            {
                if(ImGui::Button("Toggle Filter Mode"))
                {
                    if(sampler->GetMagnification() == Quanta::FilterMode::Linear)
                    {
                        sampler->SetMagnification(Quanta::FilterMode::Nearest);
                        sampler->SetMinification(Quanta::FilterMode::Nearest);
                    }
                    else
                    {
                        sampler->SetMagnification(Quanta::FilterMode::Linear);
                        sampler->SetMinification(Quanta::FilterMode::Linear);
                    }
                }

                ImGui::Image(sampler.get(), ImVec2((float) texture->GetWidth(), (float) texture->GetHeight()));
            }
            ImGui::End();
        }
        Quanta::ImGuiRenderer::End();

        window->SwapBuffers();

        Quanta::GraphicsDevice::SetRasterPipeline(nullptr);

        Quanta::GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f, 0);
    }

    Quanta::ImGuiRenderer::Shutdown();

    Quanta::GraphicsDevice::DeInitialize();
    Quanta::AudioDevice::DeInitialize();

    return 0;
}