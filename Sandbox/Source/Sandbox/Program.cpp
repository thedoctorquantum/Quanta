#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

using namespace Quanta;

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
        std::cerr << exception.what() << std::endl;
    }
    
    return contents.str();
}

int main()
{
    std::shared_ptr<Window> window = Window::Create();
    
    GraphicsDevice::Initialize(GraphicsApi::OpenGL);
    AudioDevice::Initialize(AudioApi::OpenAL);

    window->SetState(WindowState::Maximized);

    std::shared_ptr<AudioBuffer> audioBuf = AudioBuffer::FromFile("Resources/Sounds/bounce.wav");

    std::shared_ptr<AudioSource> audioSource = AudioSource::Create();

    AudioDevice::Play(*audioSource, *audioBuf);

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

    std::shared_ptr<VertexArray> vertexArray = VertexArray::Create();

    std::shared_ptr<GraphicsBuffer> vertexBuffer = GraphicsBuffer::Create(BufferUsage::Static, sizeof(vertices));
    std::shared_ptr<GraphicsBuffer> indexBuffer = GraphicsBuffer::Create(BufferUsage::Static, sizeof(indices));
    
    vertexBuffer->SetData(vertices, sizeof(vertices));
    indexBuffer->SetData(indices, sizeof(indices));

    VertexLayout layout;

    layout.Add({
        BufferPrimitive::Float,
        3,
        sizeof(float),
        false
    });

    layout.Add({
        BufferPrimitive::Float,
        4,
        sizeof(float),
        false
    });

    layout.Add({
        BufferPrimitive::Float,
        2,
        sizeof(float),
        false
    });

    vertexArray->SetVertexBuffer(vertexBuffer, layout);
    vertexArray->SetIndexBuffer(indexBuffer, IndexType::UInt8);

    std::string vertexCode = ReadAllText("Resources/Shaders/vertex.glsl");
    std::string fragmentCode = ReadAllText("Resources/Shaders/fragment.glsl");
    
    RasterPipelineDescription desc;
    
    desc.ShaderModules.emplace_back(ShaderModule::Create(ShaderType::Vertex, vertexCode));
    desc.ShaderModules.emplace_back(ShaderModule::Create(ShaderType::Fragment, fragmentCode));
    
    std::shared_ptr<GraphicsBuffer> uniforms = GraphicsBuffer::Create(BufferUsage::Static, sizeof(glm::mat4) * 2);

    desc.UniformBuffers.emplace_back(uniforms);
    
    std::shared_ptr<RasterPipeline> pipeline = RasterPipeline::Create(desc);
    
    pipeline->SetPolygonFillMode(PolygonFillMode::Solid);   
    pipeline->SetFaceCullMode(FaceCullMode::Back);
    pipeline->SetDepthTestMode(DepthTestMode::None);
    pipeline->SetEnableDepthWriting(true);
    pipeline->SetBlendMode(BlendMode::Add);

    glm::vec3 translation = glm::vec3(0.0f);

    ImGuiRenderer::Initialize(*window);
    
    std::shared_ptr<Texture2D> texture = Texture2D::FromFile("Resources/Textures/wood_floor.png");

    while(window->Exists())
    {
        window->PollEvents();

        GraphicsDevice::SetViewport({ 0.0f, 0.0f, window->GetWidth(), window->GetHeight() });

        glm::mat4 proj = glm::ortho(0.0f, (float) window->GetWidth(), 0.0f, (float) window->GetHeight(), 0.1f, 100.0f);

        uniforms->SetData(&proj, sizeof(glm::mat4), sizeof(glm::mat4));

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, translation);

        uniforms->SetData(&model, sizeof(glm::mat4));
        
        GraphicsDevice::SetRasterPipeline(pipeline);
        GraphicsDevice::SetVertexArray(vertexArray);

        GraphicsDevice::BindTexture2D(*texture, 0);
        
        DrawCommand cmd;
        
        cmd.Count = 3;
        cmd.IndexOffset = 0;
        cmd.InstanceCount = 1;
        cmd.StartInstance = 0;
        cmd.StartVertex = 0;

        GraphicsDevice::DispatchDraw(cmd);

        ImGuiRenderer::Begin(1.0f / 60.0f);
        {
            ImGui::ShowDemoWindow();
            ImGui::ShowMetricsWindow();

            ImGui::DragFloat3("Pos", &translation.x, 0.025f);
        }
        ImGuiRenderer::End();

        window->SwapBuffers();

        GraphicsDevice::SetRasterPipeline(nullptr);

        GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f, 0);
    }

    ImGuiRenderer::Shutdown();

    GraphicsDevice::DeInitialize();
    AudioDevice::DeInitialize();

    return 0;
}