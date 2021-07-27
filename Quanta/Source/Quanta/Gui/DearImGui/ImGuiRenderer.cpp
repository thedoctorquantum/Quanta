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
    static std::shared_ptr<RasterPipeline> pipeline = nullptr;
    static std::shared_ptr<VertexArray> vertexArray = nullptr;
    static std::shared_ptr<GraphicsBuffer> vertexBuffer = nullptr;
    static std::shared_ptr<GraphicsBuffer> indexBuffer = nullptr;
    static std::shared_ptr<GraphicsBuffer> uniformBuffer = nullptr;
    static std::shared_ptr<Texture2D> fontTexture = nullptr;

    static Window* currentWindow;

    static ImGuiContext* context;
    static ImGuiIO* io;

    void OnKeyDown(Key key)
    {
        io->KeysDown[(int32_t) key] = true;
    }

    void OnKeyUp(Key key)
    {
        io->KeysDown[(int32_t) key] = false;
    }

    void OnMouseDown(MouseButton button)
    {
        io->MouseDown[(int32_t) button] = true;
    }

    void OnMouseUp(MouseButton button)
    {
        io->MouseDown[(int32_t) button] = false;
    }
    
    void OnMouseMove(glm::vec2 position)
    {
        io->MousePos = *(ImVec2*) &position;
    }

    void OnMouseScroll(glm::vec2 scroll)
    {
        io->MouseWheel = scroll.y;
        io->MouseWheelH = scroll.x;
    }

    void OnCharacterDown(char character)
    {
        io->AddInputCharacter(character);
    }
    
    void ImGuiRenderer::Initialize(Window& window)
    {
        currentWindow = (Window*) &window;

        context = ImGui::CreateContext();

        ImGui::SetCurrentContext(context);

        io = &ImGui::GetIO();

        io->Fonts->AddFontDefault();

        io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        window.AddKeyDownCallback(OnKeyDown);
        window.AddKeyUpCallback(OnKeyUp);
        window.AddMouseDownCallback(OnMouseDown);
        window.AddMouseUpCallback(OnMouseUp);
        window.AddMouseMoveCallback(OnMouseMove);
        window.AddMouseScrollCallback(OnMouseScroll);
        window.AddCharacterDownCallback(OnCharacterDown);
        
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

        uniformBuffer = GraphicsBuffer::Create(BufferUsage::Static, sizeof(glm::mat4));

        RasterPipelineDescription pipelineDescription;

        pipelineDescription.ShaderModules.emplace_back(ShaderModule::Create(ShaderType::Vertex, vertexSource));
        pipelineDescription.ShaderModules.emplace_back(ShaderModule::Create(ShaderType::Fragment, fragmentSource));

        pipelineDescription.UniformBuffers.emplace_back(uniformBuffer);
        
        pipeline = RasterPipeline::Create(pipelineDescription);
        
        pipeline->SetBlendMode(BlendMode::Add);
        pipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        pipeline->SetEnableScissorTesting(true);

        vertexArray = VertexArray::Create();

        vertexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0 * sizeof(ImDrawVert));
        indexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0 * sizeof(uint16_t));

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

        io->KeyMap[ImGuiKey_Tab] = (int) Key::Tab;
        io->KeyMap[ImGuiKey_LeftArrow] = (int) Key::Left;
        io->KeyMap[ImGuiKey_RightArrow] = (int) Key::Right;
        io->KeyMap[ImGuiKey_UpArrow] = (int) Key::Up;
        io->KeyMap[ImGuiKey_DownArrow] = (int) Key::Down;
        io->KeyMap[ImGuiKey_PageUp] = (int) Key::PageUp;
        io->KeyMap[ImGuiKey_PageDown] = (int) Key::PageDown;
        io->KeyMap[ImGuiKey_Home] = (int) Key::Home;
        io->KeyMap[ImGuiKey_End] = (int) Key::End;
        io->KeyMap[ImGuiKey_Delete] = (int) Key::Delete;
        io->KeyMap[ImGuiKey_Backspace] = (int) Key::Backspace;
        io->KeyMap[ImGuiKey_Enter] = (int) Key::Enter;
        io->KeyMap[ImGuiKey_Escape] = (int) Key::Escape;
        io->KeyMap[ImGuiKey_A] = (int) Key::A;
        io->KeyMap[ImGuiKey_C] = (int) Key::C;
        io->KeyMap[ImGuiKey_V] = (int) Key::V;
        io->KeyMap[ImGuiKey_X] = (int) Key::X;
        io->KeyMap[ImGuiKey_Y] = (int) Key::Y;
        io->KeyMap[ImGuiKey_Z] = (int) Key::Z;
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
            GraphicsBuffer::Resize(*vertexBuffer, totalVertexBuffersize * 2);
        }
    
        if(totalIndexBufferSize > indexBuffer->GetSize())
        {
            GraphicsBuffer::Resize(*indexBuffer, totalIndexBufferSize * 2);
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