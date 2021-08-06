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

            vec3 viewPosition;
        } u_Matrices;

        layout(location = 0) in vec3 a_Translation;
        layout(location = 1) in vec3 a_Normal;
        layout(location = 2) in vec2 a_Uv;
        layout(location = 3) in vec3 a_Tangent;
        layout(location = 4) in vec3 a_BiTangent;
        layout(location = 5) in vec4 a_Color;

        layout(location = 0) out Out
        {
            mat3 tbn;
            vec2 uv;
            vec4 color;

            vec3 fragmentPosition;
            vec3 viewPosition;
        } v_Out;

        void main()
        {
            mat3 normalMatrix = transpose(inverse(mat3(u_Matrices.model)));

            vec3 t = normalize(normalMatrix * a_Tangent);
            vec3 n = normalize(normalMatrix * a_Normal);

            t = normalize(t - dot(t, n) * n);

            vec3 b = cross(n, t);

            v_Out.tbn = transpose(mat3(t, b, n));
            v_Out.uv = a_Uv;
            v_Out.color = a_Color;

            vec4 translation = u_Matrices.model * vec4(a_Translation, 1.0);

            v_Out.fragmentPosition = v_Out.tbn * vec3(translation);
            v_Out.viewPosition = v_Out.tbn * u_Matrices.viewPosition;

            gl_Position = u_Matrices.viewProjection * translation;
        }
    )";
    
    constexpr const char* fragmentShaderSource = R"(
        #version 450

        struct PointLight
        {
            vec3 position;

            vec4 ambient;
            vec4 diffuse;
            vec4 specular;

            float constant;
            float linear;
            float quadratic;
        };

        layout(std140, binding = 1) uniform Material
        {
            vec4 albedo;
            vec4 diffuse;
            vec4 specular;
            float shininess;
        } u_Material;  

        layout(std430, binding = 0) buffer Lights
        {
            PointLight lights[];
        };

        layout(binding = 0) uniform sampler2D u_AlbedoSampler;
        layout(binding = 1) uniform sampler2D u_DiffuseSampler;
        layout(binding = 2) uniform sampler2D u_SpecularSampler;
        layout(binding = 3) uniform sampler2D u_NormalSampler;
        
        layout(location = 0) in Out
        {
            mat3 tbn;
            vec2 uv;
            vec4 color;

            vec3 fragmentPosition;
            vec3 viewPosition;
        } v_In;

        layout(location = 0) out vec4 a_Fragment;
        
        vec4 CalculatePointLight(vec3 normal, vec3 viewDirection, vec4 albedo, vec4 diffuse, vec4 specular)
        {
            vec4 result = vec4(0.0);

            int count = lights.length();
            
            for(int i = 0; i < count; i++)
            {
                PointLight light = lights[i];

                vec3 positionDifference = (v_In.tbn * light.position) - v_In.fragmentPosition;
                vec3 lightDirection = normalize(positionDifference);
                vec3 reflectDirection = reflect(-lightDirection, normal);
                vec3 halfwayDirection = normalize(lightDirection + viewDirection);

                float diffuseFactor = max(dot(normal, lightDirection), 0.0);
                float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess);

                vec4 ambient = u_Material.albedo * albedo * light.ambient;
                vec4 diffuse = u_Material.diffuse * diffuse * light.diffuse * diffuseFactor;
                vec4 specular = u_Material.specular * specular * light.specular * specularFactor;

                float distance = length(positionDifference);
                float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

                ambient *= attenuation;
                diffuse *= attenuation;
                specular *= attenuation;

                result += ambient + diffuse + specular;
            }
            
            result.w = 1.0;

            return result;
        }

        void main()
        {
            vec3 viewDirection = normalize(v_In.viewPosition - v_In.fragmentPosition);

            vec4 sampledAlbedo = texture(u_AlbedoSampler, v_In.uv);
            vec4 sampledDiffuse = texture(u_DiffuseSampler, v_In.uv);
            vec4 sampledSpecular = texture(u_SpecularSampler, v_In.uv);
            vec4 sampledNormal = texture(u_NormalSampler, v_In.uv);

            vec3 normal = normalize(vec3(sampledNormal) * 2.0 - 1.0);

            a_Fragment = v_In.color;

            a_Fragment *= CalculatePointLight(
                normal,
                viewDirection,
                sampledAlbedo,
                sampledDiffuse,
                sampledSpecular
            );
        }
    )";

    struct State 
    {
        const Window* window = nullptr;

        std::shared_ptr<RasterPipeline> pipeline = nullptr;

        std::shared_ptr<GraphicsBuffer> matrixUniforms = nullptr;
        std::shared_ptr<GraphicsBuffer> materialUniforms = nullptr;

        std::shared_ptr<GraphicsBuffer> lightBuffer = nullptr;

        std::shared_ptr<Sampler2D> defaultAlbedoSampler = nullptr;
        std::shared_ptr<Sampler2D> defaultDiffuseSampler = nullptr;
        std::shared_ptr<Sampler2D> defaultSpecularSampler = nullptr;
        std::shared_ptr<Sampler2D> defaultNormalSampler = nullptr;

        std::vector<PointLight> lights;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);

        glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);
    } static* state;
    
    void Initialize(const Window& window)
    {
        state = new State;

        state->window = &window;

        state->matrixUniforms = GraphicsBuffer::Create(BufferUsage::Static, (sizeof(glm::mat4) * 4) + sizeof(glm::vec4));
        state->materialUniforms = GraphicsBuffer::Create(BufferUsage::Static, (sizeof(glm::vec4) * 3) + sizeof(glm::vec4));

        state->lightBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0);

        RasterPipelineDescription pipelineDescriptor;

        pipelineDescriptor.UniformBuffers.push_back(state->matrixUniforms);
        pipelineDescriptor.UniformBuffers.push_back(state->materialUniforms);

        pipelineDescriptor.StorageBuffers.push_back(state->lightBuffer);

        pipelineDescriptor.ShaderModules.push_back(ShaderModule::Create(ShaderType::Vertex, vertexShaderSource));
        pipelineDescriptor.ShaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, fragmentShaderSource));

        state->pipeline = RasterPipeline::Create(pipelineDescriptor);

        state->pipeline->SetFaceCullMode(FaceCullMode::Back);
        state->pipeline->SetEnableDepthWriting(true);
        state->pipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);
        state->pipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        state->pipeline->SetBlendMode(BlendMode::Add);

        Color32 albedo = 0xFFFFFFFF;
        Color32 diffuse = 0x99999999;
        Color32 specular = 0x00000000;
        Color32 normal = Color32(128, 128, 255, 255);

        std::shared_ptr<Texture2D> albedoTexture = Texture2D::Create(1, 1);
        std::shared_ptr<Texture2D> diffuseTexture = Texture2D::Create(1, 1);
        std::shared_ptr<Texture2D> specularTexture = Texture2D::Create(1, 1);
        std::shared_ptr<Texture2D> normalTexture = Texture2D::Create(1, 1);
        
        albedoTexture->SetData(&albedo);
        diffuseTexture->SetData(&diffuse);
        specularTexture->SetData(&specular);
        normalTexture->SetData(&normal);

        state->defaultAlbedoSampler = Sampler2D::Create(albedoTexture);

        state->defaultAlbedoSampler->SetMagnification(FilterMode::Nearest);
        state->defaultAlbedoSampler->SetMinification(FilterMode::Nearest);

        state->defaultDiffuseSampler = Sampler2D::Create(diffuseTexture);
        
        state->defaultDiffuseSampler->SetMagnification(FilterMode::Nearest);
        state->defaultDiffuseSampler->SetMinification(FilterMode::Nearest);

        state->defaultSpecularSampler = Sampler2D::Create(specularTexture);

        state->defaultSpecularSampler->SetMagnification(FilterMode::Nearest);
        state->defaultSpecularSampler->SetMinification(FilterMode::Nearest);   

        state->defaultNormalSampler = Sampler2D::Create(normalTexture);
        
        state->defaultNormalSampler->SetMagnification(FilterMode::Nearest);
        state->defaultNormalSampler->SetMinification(FilterMode::Nearest);
    }
    
    void DeInitialize()
    {
        DEBUG_ASSERT(state != nullptr);

        delete state;
    }

    const glm::mat4& GetView()
    {
        DEBUG_ASSERT(state != nullptr);

        return state->viewMatrix;
    }

    void SetView(const glm::mat4& matrix, const glm::vec3& position)
    {
        DEBUG_ASSERT(state != nullptr);

        state->viewMatrix = matrix;
        
        state->viewProjectionMatrix = state->projectionMatrix * state->viewMatrix;

        state->matrixUniforms->SetData(&state->viewMatrix, sizeof(glm::mat4), sizeof(glm::mat4));
        state->matrixUniforms->SetData(&state->viewProjectionMatrix, sizeof(glm::mat4), sizeof(glm::mat4) * 3);
        state->matrixUniforms->SetData(&position, sizeof(glm::vec3), sizeof(glm::mat4) * 4);
    }
    
    void BeginPass()
    {
        DEBUG_ASSERT(state != nullptr);

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

    void SetPointLights(const PointLight* lights, size_t count)
    {
        DEBUG_ASSERT(lights != nullptr);
        DEBUG_ASSERT(state != nullptr);

        size_t size = count * sizeof(PointLight);

        if(size > state->lightBuffer->GetSize())
        {
            GraphicsBuffer::Resize(*state->lightBuffer, size);
        }

        state->lightBuffer->SetData(lights, size);
    }
    
    void DrawMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform)
    {
        DEBUG_ASSERT(state != nullptr);

        const std::shared_ptr<VertexArray>& vertexArray = mesh.GetVertexArray();

        DEBUG_ASSERT(vertexArray != nullptr);

        float shininess = material.GetShininess();

        state->materialUniforms->SetData(&material.GetAlbedo(), sizeof(glm::vec4));
        state->materialUniforms->SetData(&material.GetDiffuse(), sizeof(glm::vec4), sizeof(glm::vec4));        
        state->materialUniforms->SetData(&material.GetSpecular(), sizeof(glm::vec4), sizeof(glm::vec4) * 2);
        state->materialUniforms->SetData(&shininess, sizeof(float), sizeof(glm::vec4) * 3);
        
        state->matrixUniforms->SetData(&transform, sizeof(glm::mat4));

        GraphicsDevice::SetVertexArray(vertexArray);

        if(material.GetAlbedoSampler() != nullptr)
        {
            GraphicsDevice::BindSampler(*material.GetAlbedoSampler(), 0);
        }
        else
        {
            GraphicsDevice::BindSampler(*state->defaultAlbedoSampler, 0);
        }

        if(material.GetDiffuseSampler() != nullptr)
        {
            GraphicsDevice::BindSampler(*material.GetDiffuseSampler(), 1);
        }
        else
        {
            GraphicsDevice::BindSampler(*state->defaultDiffuseSampler, 1);
        }

        if(material.GetSpecularSampler() != nullptr)
        {
            GraphicsDevice::BindSampler(*material.GetSpecularSampler(), 2);
        }
        else
        {
            GraphicsDevice::BindSampler(*state->defaultSpecularSampler, 2);
        }

        if(material.GetNormalSampler() != nullptr)
        {
            GraphicsDevice::BindSampler(*material.GetNormalSampler(), 3);
        }
        else
        {
            GraphicsDevice::BindSampler(*state->defaultNormalSampler, 3);
        }
        
        DrawCommand command;

        command.Count = mesh.GetIndexCount();

        GraphicsDevice::DispatchDraw(command);
    }
}