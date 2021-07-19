#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <stb_image.h>
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

int main(int argc, char** args)
{
    Window window = Window("Sandbox", { 640, 480 });
    
    float vertices[3 * 7] = 
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
    
    uint32_t indices[3] =
    {
        0, 1, 2
    };
    
    std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();

    std::shared_ptr<GraphicsBuffer> vertexBuffer = std::make_shared<GraphicsBuffer>(BufferUsage::Static, sizeof(vertices));
    std::shared_ptr<GraphicsBuffer> indexBuffer = std::make_shared<GraphicsBuffer>(BufferUsage::Static, sizeof(indices));
    
    vertexBuffer->SetData(vertices, sizeof(vertices));
    indexBuffer->SetData(indices, sizeof(indices));

    VertexLayout layout;

    layout.Add({ 
        .Primitve = BufferPrimitive::Float,
        .Count = 3,
        .Size = sizeof(float),
        .Normalized = false
    });
    
    layout.Add({ 
        .Primitve = BufferPrimitive::Float,
        .Count = 4,
        .Size = sizeof(float),
        .Normalized = false
    });

    vertexArray->SetVertexBuffer(vertexBuffer, layout);
    vertexArray->SetIndexBuffer(indexBuffer, IndexType::UInt32);

    std::string vertexCode = ReadAllText("Resources/Shaders/vertex.glsl");
    std::string fragmentCode = ReadAllText("Resources/Shaders/fragment.glsl");
    
    std::vector<std::shared_ptr<ShaderModule>> modules;

    modules.emplace_back(std::make_shared<ShaderModule>(ShaderType::Vertex, vertexCode));
    modules.emplace_back(std::make_shared<ShaderModule>(ShaderType::Fragment, fragmentCode));

    std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>(modules);

    while(window.Exists())
    {
        window.PollEvents();

        GraphicsDevice::ClearBackBuffer({ 0.0f, 0.5f, 1.0f, 1.0f }, 1.0f, 0);
        GraphicsDevice::Viewport({ 0, 0, 640, 480 });

        GraphicsDevice::SetPipeline(program);

        GraphicsDevice::SetVertexArray(vertexArray);

        DrawCommand cmd;

        cmd.Count = 3;
        cmd.IndexOffset = 0;
        cmd.InstanceCount = 1;
        cmd.StartInstance = 0;
        cmd.StartVertex = 0;
        
        GraphicsDevice::DispatchDraw(cmd);

        window.SwapBuffers();
    }
    
    return 0;
}