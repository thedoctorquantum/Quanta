#include <Quanta/Gui/DearImGui/ImGuiRenderer.h>
#include <Quanta/Graphics/Buffer/GraphicsBuffer.h>
#include <Quanta/Graphics/Buffer/VertexArray.h>
#include <Quanta/Graphics/Pipeline/RasterPipeline.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <Quanta/Graphics/Texture/Texture2D.h>
#include <Quanta/Graphics/Texture/Sampler2D.h>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>

namespace Quanta
{
    struct State
    {
        std::shared_ptr<RasterPipeline> pipeline = nullptr;
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::shared_ptr<GraphicsBuffer> vertexBuffer = nullptr;
        std::shared_ptr<GraphicsBuffer> indexBuffer = nullptr;
        std::shared_ptr<GraphicsBuffer> uniformBuffer = nullptr;
        std::shared_ptr<Texture2D> fontTexture = nullptr;
        std::shared_ptr<Sampler2D> fontSampler = nullptr;
    
        Window* window = nullptr;

        ImGuiContext* context = nullptr;
        ImGuiIO* io = nullptr;
    } static* state;

    void OnKeyDown(Key key)
    {
       state->io->KeysDown[(int32_t) key] = true;
    }

    void OnKeyUp(Key key)
    {
        state->io->KeysDown[(int32_t) key] = false;
    }

    void OnMouseDown(MouseButton button)
    {
        state->io->MouseDown[(int32_t) button] = true;
    }

    void OnMouseUp(MouseButton button)
    {
        state->io->MouseDown[(int32_t) button] = false;
    }
    
    void OnMouseMove(glm::vec2 position)
    {
        state->io->MousePos = *(ImVec2*) &position;
    }

    void OnMouseScroll(glm::vec2 scroll)
    {
        state->io->MouseWheel = scroll.y;
        state->io->MouseWheelH = scroll.x;
    }

    void OnCharacterDown(char character)
    {
        state->io->AddInputCharacter(character);
    }
    
    void ImGuiRenderer::Initialize(Window& window)
    {
        state = new State;

        state->window = (Window*) &window;

        state->context = ImGui::CreateContext();

        ImGui::SetCurrentContext(state->context);

        state->io = &ImGui::GetIO();

        state->io->Fonts->AddFontDefault();

        state->io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        state->io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

        state->uniformBuffer = GraphicsBuffer::Create(BufferUsage::Static, sizeof(glm::mat4));

        RasterPipelineDescription pipelineDescription;

        pipelineDescription.ShaderModules.emplace_back(ShaderModule::Create(ShaderType::Vertex, vertexSource));
        pipelineDescription.ShaderModules.emplace_back(ShaderModule::Create(ShaderType::Pixel, fragmentSource));

        pipelineDescription.UniformBuffers.emplace_back(state->uniformBuffer);
        
        state->pipeline = RasterPipeline::Create(pipelineDescription);
        
        state->pipeline->SetBlendMode(BlendMode::Add);
        state->pipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        state->pipeline->SetEnableScissorTesting(true);

        state->vertexArray = VertexArray::Create();

        state->vertexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0 * sizeof(ImDrawVert));
        state->indexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0 * sizeof(uint16_t));

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

        state->vertexArray->SetVertexBuffer(state->vertexBuffer, layout);
        state->vertexArray->SetIndexBuffer(state->indexBuffer, IndexType::UInt16);

        uint8_t* pixels;
        uint32_t width;
        uint32_t height;

        state->io->Fonts->GetTexDataAsRGBA32(&pixels, (int*) &width, (int*) &height);

        state->fontTexture = Texture2D::Create(width, height);

        state->fontTexture->SetData(pixels);

        state->fontSampler = Sampler2D::Create(state->fontTexture);

        state->fontSampler->SetMagnification(FilterMode::Nearest);
        state->fontSampler->SetMinification(FilterMode::Nearest);

        state->io->Fonts->SetTexID(state->fontSampler.get());

        state->io->Fonts->ClearTexData();

        state->io->KeyMap[ImGuiKey_Tab] = (int) Key::Tab;
        state->io->KeyMap[ImGuiKey_LeftArrow] = (int) Key::Left;
        state->io->KeyMap[ImGuiKey_RightArrow] = (int) Key::Right;
        state->io->KeyMap[ImGuiKey_UpArrow] = (int) Key::Up;
        state->io->KeyMap[ImGuiKey_DownArrow] = (int) Key::Down;
        state->io->KeyMap[ImGuiKey_PageUp] = (int) Key::PageUp;
        state->io->KeyMap[ImGuiKey_PageDown] = (int) Key::PageDown;
        state->io->KeyMap[ImGuiKey_Home] = (int) Key::Home;
        state->io->KeyMap[ImGuiKey_End] = (int) Key::End;
        state->io->KeyMap[ImGuiKey_Delete] = (int) Key::Delete;
        state->io->KeyMap[ImGuiKey_Backspace] = (int) Key::Backspace;
        state->io->KeyMap[ImGuiKey_Enter] = (int) Key::Enter;
        state->io->KeyMap[ImGuiKey_Escape] = (int) Key::Escape;
        state->io->KeyMap[ImGuiKey_A] = (int) Key::A;
        state->io->KeyMap[ImGuiKey_C] = (int) Key::C;
        state->io->KeyMap[ImGuiKey_V] = (int) Key::V;
        state->io->KeyMap[ImGuiKey_X] = (int) Key::X;
        state->io->KeyMap[ImGuiKey_Y] = (int) Key::Y;
        state->io->KeyMap[ImGuiKey_Z] = (int) Key::Z;
    }

    void ImGuiRenderer::Shutdown()
    {
        delete state;
    }

    void ImGuiRenderer::Begin(float elapsed)
    {
        state->io->DisplaySize = ImVec2((float) state->window->GetWidth(), (float) state->window->GetHeight());
        state->io->DeltaTime = elapsed;
        state->io->DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        state->pipeline->SetViewport({ 0.0f, 0.0f, state->window->GetWidth(), state->window->GetHeight() });

        ImGui::NewFrame();
    }
    
    void ImGuiRenderer::End()
    {
        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();

        if(!drawData->CmdListsCount) return;

        drawData->ScaleClipRects(state->io->DisplayFramebufferScale);

        glm::mat4 matrix = glm::ortho(0.0f, state->io->DisplaySize.x, state->io->DisplaySize.y, 0.0f, -1.0f, 1.0f);

        state->uniformBuffer->SetData(&matrix, sizeof(glm::mat4));

        GraphicsDevice::SetRasterPipeline(state->pipeline);
        GraphicsDevice::SetVertexArray(state->vertexArray);

        size_t totalVertexBuffersize = drawData->TotalVtxCount * sizeof(ImDrawVert);
        size_t totalIndexBufferSize = drawData->TotalIdxCount * sizeof(uint16_t);

        if(totalVertexBuffersize > state->vertexBuffer->GetSize())
        {
            GraphicsBuffer::Resize(*state->vertexBuffer, totalVertexBuffersize * 2);
        }
    
        if(totalIndexBufferSize > state->indexBuffer->GetSize())
        {
            GraphicsBuffer::Resize(*state->indexBuffer, totalIndexBufferSize * 2);
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

            state->vertexBuffer->SetData(drawList->VtxBuffer.Data, vertexBufferSize, vertexOffset);
            state->indexBuffer->SetData(drawList->IdxBuffer.Data, indexBufferSize, indexOffset);

            for(int32_t j = 0; j < commands.Size; j++)
            {
                ImDrawCmd& command = commands[j];

                Sampler2D* sampler = (Sampler2D*) command.TextureId;

                GraphicsDevice::BindSampler2D(*sampler, 0);

                state->pipeline->SetScissorViewport({
                     (uint32_t) command.ClipRect.x, 
                     (uint32_t) (state->window->GetHeight() - command.ClipRect.w), 
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