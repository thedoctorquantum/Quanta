#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>

#include "ImGuiRenderer.h"

#include "../../Graphics/Rendering/Buffer/GraphicsBuffer.h"
#include "../../Graphics/Rendering/Buffer/VertexArray.h"
#include "../../Graphics/Rendering/Pipeline/RasterPipeline.h"
#include "../../Graphics/Rendering/GraphicsDevice.h"
#include "../../Graphics/Rendering/Texture/Texture.h"

namespace Quanta
{
    static std::shared_ptr<RasterPipeline> pipeline;
    static std::shared_ptr<VertexArray> vertexArray;
    static std::shared_ptr<GraphicsBuffer> vertexBuffer;
    static std::shared_ptr<GraphicsBuffer> indexBuffer;
    static std::shared_ptr<GraphicsBuffer> uniformBuffer;
    static std::shared_ptr<Texture2D> fontTexture;

    static Window* currentWindow;

    static ImGuiContext* context;
    static ImGuiIO* io;

    void ImGuiRenderer::Initialize(const Window& window)
    {
        currentWindow = (Window*) &window;

        context = ImGui::CreateContext();

        ImGui::SetCurrentContext(context);

        io = &ImGui::GetIO();

        io->Fonts->AddFontDefault();

        io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        std::string vertexSource =
            R"(
            #version 450 core
            
            layout(location = 0) in vec2 a_Position;
            layout(location = 1) in vec2 a_Uv;
            layout(location = 2) in vec4 a_Color;
            
            layout(location = 0) out Out
            {
                vec4 color;
                vec2 uv;
            } v_Out;
            
            layout(std140, binding = 0) uniform Matrices
            {
                mat4 matrix;
            } u_Matrices;
            
            void main()
            {
                v_Out.color = a_Color;
                v_Out.uv = a_Uv;

                gl_Position = u_Matrices.matrix * vec4(a_Position, 0.0, 1.0);
            })";
            
        std::string fragmentSource =
            R"(
            #version 450 core

            layout(location = 0) out vec4 a_Fragment;

            layout(location = 0) in Out
            {
                vec4 color;
                vec2 uv;
            } v_In;
            
            layout(binding = 0) uniform sampler2D u_Sampler;

            void main()
            {
                a_Fragment = v_In.color * texture(u_Sampler, v_In.uv);
            })";
        
        uniformBuffer = std::make_shared<GraphicsBuffer>(BufferUsage::Static, sizeof(glm::mat4));

        RasterPipelineDescription pipelineDescription;

        pipelineDescription.ShaderModules.emplace_back(std::make_shared<ShaderModule>(ShaderType::Vertex, vertexSource));
        pipelineDescription.ShaderModules.emplace_back(std::make_shared<ShaderModule>(ShaderType::Fragment, fragmentSource));

        pipelineDescription.UniformBuffers.emplace_back(uniformBuffer);
        
        pipeline = std::make_shared<RasterPipeline>(pipelineDescription);

        pipeline->SetBlendMode(BlendMode::Add);
        pipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        pipeline->SetEnableScissorTesting(true);
        pipeline->SetPolygonFillMode(PolygonFillMode::Solid);

        vertexArray = std::make_shared<VertexArray>();

        vertexBuffer = std::make_shared<GraphicsBuffer>(BufferUsage::Dynamic, 1000 * sizeof(ImDrawVert));
        indexBuffer = std::make_shared<GraphicsBuffer>(BufferUsage::Dynamic, 1000 * sizeof(uint16_t));

        VertexLayout layout;

        for(int32_t i = 0; i < 2; i++)
        {
            layout.Add({
                BufferPrimitive::Float,
                2,
                sizeof(float),
                false
            });
        }
        
        layout.Add(
        {
            BufferPrimitive::UInt8,
            4,
            sizeof(uint8_t),
            true
        });

        vertexArray->SetVertexBuffer(vertexBuffer, layout);
        vertexArray->SetIndexBuffer(indexBuffer, IndexType::UInt16);

        uint8_t* pixels;
        uint32_t width;
        uint32_t height;

        io->Fonts->GetTexDataAsRGBA32(&pixels, (int*) &width, (int*) &height);

        fontTexture = std::make_shared<Texture2D>(width, height);

        fontTexture->SetData(pixels);

        io->Fonts->SetTexID((void*) (size_t) fontTexture->GetHandle());

        io->Fonts->ClearTexData();
    }

    void ImGuiRenderer::Shutdown()
    {
        pipeline.reset((RasterPipeline*) nullptr);
        vertexArray.reset((VertexArray*) nullptr);
        vertexBuffer.reset((GraphicsBuffer*) nullptr);
        indexBuffer.reset((GraphicsBuffer*) nullptr);
        uniformBuffer.reset((GraphicsBuffer*) nullptr);
        fontTexture.reset((Texture2D*) nullptr);
    }

    void ImGuiRenderer::Begin(float elapsed)
    {
        io->DisplaySize = ImVec2((float) currentWindow->GetWidth(), (float) currentWindow->GetHeight());
        io->DeltaTime = elapsed;
        io->DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        ImGui::NewFrame();
    }
    
    void ImGuiRenderer::End()
    {
        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();

        if(!drawData->CmdListsCount) return;

        drawData->ScaleClipRects(io->DisplayFramebufferScale);

        glm::mat4 matrix = glm::ortho(0.0f, io->DisplaySize.x, io->DisplaySize.y, 0.0f, -1.0f, 1.0f);

        uniformBuffer->SetData(&matrix, sizeof(glm::mat4));

        GraphicsDevice::SetRasterPipeline(pipeline);
        GraphicsDevice::SetVertexArray(vertexArray);

        size_t totalVertexBuffersize = drawData->TotalVtxCount * sizeof(ImDrawVert);
        size_t totalIndexBufferSize = drawData->TotalIdxCount * sizeof(uint16_t);

        if(totalVertexBuffersize > vertexBuffer->GetSize())
        {
            GraphicsBuffer::Resize(*vertexBuffer, totalVertexBuffersize);
        }

        if(totalIndexBufferSize > indexBuffer->GetSize())
        {
            GraphicsBuffer::Resize(*indexBuffer, totalIndexBufferSize);
        }
        
        size_t vertexOffset = 0;
        size_t indexOffset = 0;

        size_t startVertex = 0;

        DrawCommand drawCommand;

        for(int32_t i = 0; i < drawData->CmdListsCount; i++)
        {
            ImDrawList* drawList = drawData->CmdLists[i];

            ImVector<ImDrawCmd>& commands = drawList->CmdBuffer;

            size_t vertexBufferSize = drawList->VtxBuffer.Size * sizeof(ImDrawVert);
            size_t indexBufferSize = drawList->IdxBuffer.Size * sizeof(uint16_t);

            vertexBuffer->SetData(drawList->VtxBuffer.Data, vertexBufferSize, vertexOffset);
            indexBuffer->SetData(drawList->IdxBuffer.Data, indexBufferSize, indexOffset);

            for(int32_t j = 0; j < commands.Size; j++)
            {
                ImDrawCmd& command = commands[j];

                GraphicsDevice::BindTexture((uint32_t) (size_t) command.TextureId, 0);

                GraphicsDevice::SetScissorViewport({
                     (uint32_t) command.ClipRect.x, 
                     (uint32_t) (currentWindow->GetHeight() - command.ClipRect.w), 
                     (uint32_t) (command.ClipRect.z - command.ClipRect.x),
                     (uint32_t) (command.ClipRect.w - command.ClipRect.y) 
                });

                drawCommand.Count = command.ElemCount;
                drawCommand.IndexOffset = indexOffset;
                drawCommand.StartVertex = startVertex;

                GraphicsDevice::DispatchDraw(drawCommand);

                indexOffset += command.ElemCount * sizeof(uint16_t);
            }

            vertexOffset += vertexBufferSize;
            
            startVertex += drawList->VtxBuffer.Size;
        }
    }
}