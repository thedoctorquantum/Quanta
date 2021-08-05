#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Graphics/Pipeline/RasterPipeline.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Debugging/Validation.h"

namespace Quanta::Renderer3D
{
    constexpr const char* vertexShaderSource = R"(
        #version 450

        layout(std140, binding = 0) uniform Matrices
        {
            mat4 model;
            mat4 view;
            mat4 projection;        
            
            mat4 viewProjection;
        } u_Matrices;

        layout(location = 0) in vec3 a_Translation;
        layout(location = 1) in vec3 a_Normal;
        layout(location = 2) in vec2 a_Uv;
        layout(location = 3) in vec4 a_Color;

        layout(location = 0) out Out
        {
            vec3 normal;
            vec2 uv;
            vec4 color;
        } v_Out;

        void main()
        {
            v_Out.normal = a_Normal;
            v_Out.uv = a_Uv;
            v_Out.color = a_Color;

            gl_Position = u_Matrices.viewProjection * u_Matrices.model * vec4(a_Translation, 1.0);
        }
    )";
    
    constexpr const char* fragmentShaderSource = R"(
        #version 450

        struct Material
        {
            vec4 color;
        };
        
        layout(std140, binding = 1) uniform MaterialUniforms
        {
            Material material;
        } u_Material;  

        layout(binding = 0) uniform sampler2D u_AlbedoSampler;

        layout(location = 0) in Out
        {
            vec3 normal;
            vec2 uv;
            vec4 color;
        } v_In;

        layout(location = 0) out vec4 a_Fragment;

        void main()
        {
            a_Fragment = texture(u_AlbedoSampler, v_In.uv) * u_Material.material.color * v_In.color;
        }
    )";

    struct State 
    {
        const Window* window = nullptr;

        std::shared_ptr<RasterPipeline> pipeline = nullptr;

        std::shared_ptr<GraphicsBuffer> matrixUniforms = nullptr;
        std::shared_ptr<GraphicsBuffer> materialUniforms = nullptr;

        std::shared_ptr<Sampler2D> defaultSampler = nullptr;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);

        glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);
    } static* state;
    
    void Initialize(const Window& window)
    {
        state = new State;

        state->window = &window;

        state->matrixUniforms = GraphicsBuffer::Create(BufferUsage::Static, sizeof(glm::mat4) * 4);
        state->materialUniforms = GraphicsBuffer::Create(BufferUsage::Static, sizeof(glm::vec4));

        RasterPipelineDescription pipelineDescriptor;

        pipelineDescriptor.UniformBuffers.push_back(state->matrixUniforms);
        pipelineDescriptor.UniformBuffers.push_back(state->materialUniforms);

        pipelineDescriptor.ShaderModules.push_back(ShaderModule::Create(ShaderType::Vertex, vertexShaderSource));
        pipelineDescriptor.ShaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, fragmentShaderSource));

        state->pipeline = RasterPipeline::Create(pipelineDescriptor);

        state->pipeline->SetFaceCullMode(FaceCullMode::None);
        state->pipeline->SetEnableDepthWriting(false);
        state->pipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);
        state->pipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        state->pipeline->SetBlendMode(BlendMode::Add);

        std::shared_ptr<Texture2D> defaultTexture = Texture2D::Create(1, 1);

        Color32 white = 0xFFFFFFFF;

        defaultTexture->SetData(&white);

        state->defaultSampler = Sampler2D::Create(defaultTexture);

        state->defaultSampler->SetMagnification(FilterMode::Nearest);
        state->defaultSampler->SetMinification(FilterMode::Nearest);
    }
    
    void DeInitialize()
    {
        delete state;
    }

    const glm::mat4& GetView()
    {
        DEBUG_ASSERT(state != nullptr);

        return state->viewMatrix;
    }

    void SetView(const glm::mat4& value)
    {
        DEBUG_ASSERT(state != nullptr);

        state->viewMatrix = value;
        
        state->viewProjectionMatrix = state->projectionMatrix * state->viewMatrix;

        state->matrixUniforms->SetData(&state->viewMatrix, sizeof(glm::mat4), sizeof(glm::mat4));
        state->matrixUniforms->SetData(&state->viewProjectionMatrix, sizeof(glm::mat4), sizeof(glm::mat4) * 3);
    }
    
    void BeginPass()
    {
        state->projectionMatrix = glm::perspective(
            45.0f * (static_cast<float>(M_PI) / 180.0f),
            static_cast<float>(state->window->GetWidth()) / static_cast<float>(state->window->GetHeight()),
            0.1f, 2000.0f
        );
    
        state->matrixUniforms->SetData(&state->projectionMatrix, sizeof(glm::mat4), sizeof(glm::mat4) * 2);

        state->pipeline->SetViewport({ 0, 0, state->window->GetWidth(), state->window->GetHeight() });
    
        GraphicsDevice::SetRasterPipeline(state->pipeline);
    }

    void EndPass()
    {
        DEBUG_ASSERT(state != nullptr);

        GraphicsDevice::SetRasterPipeline(nullptr);
    }
    
    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform)
    {
        DEBUG_ASSERT(state != nullptr);

        const std::shared_ptr<VertexArray>& vertexArray = mesh.GetVertexArray();

        state->materialUniforms->SetData(&material.GetColor(), sizeof(glm::vec4));        
        state->matrixUniforms->SetData(&transform, sizeof(glm::mat4));

        GraphicsDevice::SetVertexArray(vertexArray);

        if(material.GetAlbedoSampler() != nullptr)
        {
            GraphicsDevice::BindSampler(*material.GetAlbedoSampler(), 0);
        }
        else
        {
            GraphicsDevice::BindSampler(*state->defaultSampler, 0);
        }

        DrawCommand command;

        command.Count = mesh.GetIndexCount();

        GraphicsDevice::DispatchDraw(command);
    }
}