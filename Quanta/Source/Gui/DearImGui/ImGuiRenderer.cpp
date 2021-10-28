#include <Quanta/Gui/DearImGui/ImGuiRenderer.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    struct State
    {
        std::shared_ptr<RasterPipeline> pipeline = nullptr;
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::shared_ptr<GraphicsBuffer> vertexBuffer = nullptr;
        std::shared_ptr<GraphicsBuffer> indexBuffer = nullptr;
        std::shared_ptr<GraphicsBuffer> uniformBuffer = nullptr;
        std::shared_ptr<Texture> fontTexture = nullptr;
        std::shared_ptr<Sampler> fontSampler = nullptr;
    
        std::shared_ptr<Window> window = nullptr;

        ImGuiContext* context = nullptr;
        ImGuiIO* io = nullptr;
    } static* state;
    
    static void BuildFontAtlas()
    {
        if (state->io->Fonts->IsBuilt()) return;

        unsigned char* pixels = nullptr;
        int width = 0;
        int height = 0;

        state->io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        state->fontTexture = Texture::Create(Texture::Type::Texture2D, TexelFormat::Rgba8I, width, height, 1);

        state->fontTexture->SetData(pixels);

        state->fontSampler = Sampler::Create(state->fontTexture);

        state->fontSampler->SetMagnification(FilterMode::Linear);
        state->fontSampler->SetMinification(FilterMode::Linear);

        state->io->Fonts->SetTexID(state->fontSampler.get());
    }
    
    static void OnKeyDown(const Key key)
    {
        state->io->KeysDown[static_cast<std::size_t>(key)] = true;

        state->io->KeyCtrl = 
            state->io->KeysDown[static_cast<std::size_t>(Key::LeftControl)] &&
            state->io->KeysDown[static_cast<std::size_t>(Key::RightControl)];
        
        state->io->KeyShift = 
            state->io->KeysDown[static_cast<std::size_t>(Key::LeftShift)] &&
            state->io->KeysDown[static_cast<std::size_t>(Key::RightShift)];

        state->io->KeyAlt = 
            state->io->KeysDown[static_cast<std::size_t>(Key::LeftAlt)] &&
            state->io->KeysDown[static_cast<std::size_t>(Key::RightAlt)];
        
        switch (key)
        {
            case Key::LeftControl: 
            case Key::RightControl: 
                state->io->KeyMods |= ImGuiKeyModFlags_Ctrl;

                break;
            case Key::LeftShift: 
            case Key::RightShift:
                state->io->KeyMods |= ImGuiKeyModFlags_Shift;

                break;
            case Key::LeftAlt:
            case Key::RightAlt:
                state->io->KeyMods |= ImGuiKeyModFlags_Alt;

                break;
        }
    }

    static void OnKeyUp(const Key key)
    {
        state->io->KeysDown[static_cast<size_t>(key)] = false;
    }

    static void OnMouseDown(const MouseButton button)
    {
        state->io->MouseDown[static_cast<size_t>(button)] = true;
    }

    static void OnMouseUp(const MouseButton button)
    {
        state->io->MouseDown[static_cast<size_t>(button)] = false;
    }
    
    static void OnMouseMove(const glm::vec2 position)
    {
        state->io->MousePos = { position.x, position.y };
    }

    static void OnMouseScroll(const glm::vec2 scroll)
    {
        state->io->MouseWheel = scroll.y;
        state->io->MouseWheelH = scroll.x;
    }

    static void OnCharacterDown(const char character)
    {
        state->io->AddInputCharacter(character);
    }

    static void SetClipboardText(void* const user_data, const char* const text)
    {
        state->window->SetClipboardText(text);
    }
    
    static const char* GetClipboardText(void* const user_data)
    {
        return state->window->GetClipboardText();
    }
    
    void ImGuiRenderer::Create(const std::shared_ptr<Window>& window)
    {
        state = new State;

        state->window = window;

        state->context = ImGui::CreateContext();

        DEBUG_ASSERT(state->context != nullptr);

        ImGui::SetCurrentContext(state->context);

        state->io = &ImGui::GetIO();

        DEBUG_ASSERT(state->io != nullptr);

        state->io->Fonts->AddFontDefault();

        state->io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        state->io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        state->io->SetClipboardTextFn = SetClipboardText;
        state->io->GetClipboardTextFn = GetClipboardText;

        window->AddKeyDownCallback(OnKeyDown);
        window->AddKeyUpCallback(OnKeyUp);
        window->AddMouseDownCallback(OnMouseDown);
        window->AddMouseUpCallback(OnMouseUp);
        window->AddMouseMoveCallback(OnMouseMove);
        window->AddMouseScrollCallback(OnMouseScroll);
        window->AddCharacterDownCallback(OnCharacterDown);
        
        constexpr auto vertexSource =
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
            
        constexpr auto fragmentSource =
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

        RasterPipeline::Description pipelineDescription;

        pipelineDescription.vertexShader = ShaderModule::Create(ShaderType::Vertex, vertexSource);
        pipelineDescription.fragmentShader = ShaderModule::Create(ShaderType::Pixel, fragmentSource);

        pipelineDescription.uniformBuffers.emplace_back(state->uniformBuffer);
        
        state->pipeline = RasterPipeline::Create(pipelineDescription);
        
        state->pipeline->blendMode = BlendMode::Add;
        state->pipeline->blendFactor = BlendFactor::InverseSourceAlpha;
        state->pipeline->enableScissorTesting = true;

        state->vertexArray = VertexArray::Create();

        state->vertexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0);
        state->indexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0);

        VertexLayout layout;

        for (int i = 0; i < 2; i++)
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
            sizeof(std::uint8_t),
            true
        });

        state->vertexArray->SetVertexBuffer(state->vertexBuffer, layout);
        state->vertexArray->SetIndexBuffer(state->indexBuffer, IndexType::UInt16);

        state->io->KeyMap[ImGuiKey_Tab] = static_cast<int>(Key::Tab);
        state->io->KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Key::Left);
        state->io->KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Key::Right);
        state->io->KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Key::Up);
        state->io->KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Key::Down);
        state->io->KeyMap[ImGuiKey_PageUp] = static_cast<int>(Key::PageUp);
        state->io->KeyMap[ImGuiKey_PageDown] = static_cast<int>(Key::PageDown);
        state->io->KeyMap[ImGuiKey_Home] = static_cast<int>(Key::Home);
        state->io->KeyMap[ImGuiKey_End] = static_cast<int>(Key::End);
        state->io->KeyMap[ImGuiKey_Delete] = static_cast<int>(Key::Delete);
        state->io->KeyMap[ImGuiKey_Backspace] = static_cast<int>(Key::Backspace);
        state->io->KeyMap[ImGuiKey_Enter] = static_cast<int>(Key::Enter);
        state->io->KeyMap[ImGuiKey_Escape] = static_cast<int>(Key::Escape);
        state->io->KeyMap[ImGuiKey_A] = static_cast<int>(Key::A);
        state->io->KeyMap[ImGuiKey_C] = static_cast<int>(Key::C);
        state->io->KeyMap[ImGuiKey_V] = static_cast<int>(Key::V);
        state->io->KeyMap[ImGuiKey_X] = static_cast<int>(Key::X);
        state->io->KeyMap[ImGuiKey_Y] = static_cast<int>(Key::Y);
        state->io->KeyMap[ImGuiKey_Z] = static_cast<int>(Key::Z);
    }

    void ImGuiRenderer::Destroy()
    {
        delete state;
    }

    void ImGuiRenderer::Begin(const float elapsed)
    {
        BuildFontAtlas();
        
        state->io->DisplaySize.x = state->window->GetFrameBufferSize().x;
        state->io->DisplaySize.y = state->window->GetFrameBufferSize().y; 
        
        state->io->DeltaTime = elapsed;

        ImGui::NewFrame();
    }
    
    void ImGuiRenderer::End()
    {
        ImGui::Render();

        const auto drawData = ImGui::GetDrawData();

        DEBUG_ASSERT(drawData != nullptr);

        if (drawData->CmdListsCount <= 0) return;

        drawData->ScaleClipRects(state->io->DisplayFramebufferScale);

        const auto matrix = glm::ortho(0.0f, state->io->DisplaySize.x, state->io->DisplaySize.y, 0.0f, -1.0f, 1.0f);

        state->uniformBuffer->SetData(&matrix, sizeof(glm::mat4));

        GraphicsDevice::SetViewport({ 0, 0, state->io->DisplaySize.x, state->io->DisplaySize.y });
        GraphicsDevice::SetRasterPipeline(state->pipeline.get());
        GraphicsDevice::SetVertexArray(state->vertexArray.get());

        const auto totalVertexBuffersize = drawData->TotalVtxCount * sizeof(ImDrawVert);
        const auto totalIndexBufferSize = drawData->TotalIdxCount * sizeof(uint16_t);

        if(totalVertexBuffersize > state->vertexBuffer->GetSize())
        {
            state->vertexBuffer->Resize(totalVertexBuffersize * 2);
        }
            
        if(totalIndexBufferSize > state->indexBuffer->GetSize())
        {
            state->indexBuffer->Resize(totalIndexBufferSize * 2);
        }
        
        std::size_t vertexOffset = 0;
        std::size_t indexOffset = 0;
        
        std::size_t startVertex = 0;

        DrawCommand drawCommand;

        for (std::size_t i = 0; i < drawData->CmdListsCount; i++)
        {
            const auto drawList = drawData->CmdLists[i];

            const auto& commands = drawList->CmdBuffer;

            const auto vertexBufferSize = drawList->VtxBuffer.Size * sizeof(ImDrawVert);
            const auto indexBufferSize = drawList->IdxBuffer.Size * sizeof(ImWchar);

            state->vertexBuffer->SetData(drawList->VtxBuffer.Data, vertexBufferSize, vertexOffset);
            state->indexBuffer->SetData(drawList->IdxBuffer.Data, indexBufferSize, indexOffset);
            
            for (size_t j = 0; j < commands.Size; j++)
            {
                const auto& command = commands[j];

                const auto sampler = static_cast<const Sampler*>(command.TextureId);

                DEBUG_ASSERT(sampler != nullptr);

                GraphicsDevice::BindSampler(sampler, 0);

                GraphicsDevice::SetScissorViewport({
                    static_cast<std::uint32_t>(command.ClipRect.x), 
                    static_cast<std::uint32_t>(state->window->GetHeight() - command.ClipRect.w), 
                    static_cast<std::uint32_t>(command.ClipRect.z - command.ClipRect.x),
                    static_cast<std::uint32_t>(command.ClipRect.w - command.ClipRect.y) 
                });
                
                drawCommand.Count = command.ElemCount;
                drawCommand.IndexOffset = indexOffset;
                drawCommand.StartVertex = startVertex;

                GraphicsDevice::DispatchDraw(drawCommand);

                indexOffset += command.ElemCount * sizeof(ImWchar);
            }

            vertexOffset += vertexBufferSize;
            
            startVertex += drawList->VtxBuffer.Size;
        }
    }
}