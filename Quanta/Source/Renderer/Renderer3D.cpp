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
        layout(location = 4) in vec3 a_Color;

        layout(location = 0) out Out
        {
            mat3 tbn;
            vec2 uv;
            vec3 color;

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
    
    constexpr const char* opaqueFragmentShaderSource = R"(
        #version 450

        struct PointLight
        {
            vec3 position;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;

            float constant;
            float linear;
            float quadratic;
        };

        struct DirectionalLight
        {
            vec3 direction;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        layout(std140, binding = 1) uniform Material
        {
            vec3 albedo;
            vec3 diffuse;
            vec3 specular;
            float shininess;
            float opacity;
        } u_Material;  

        layout(std140, binding = 2) uniform DirectionalLights
        {
            DirectionalLight directionalLight;
        };

        layout(std430, binding = 0) buffer PointLights
        {
            PointLight pointLights[];
        };

        layout(binding = 0) uniform sampler2D u_AlbedoSampler;
        layout(binding = 1) uniform sampler2D u_DiffuseSampler;
        layout(binding = 2) uniform sampler2D u_SpecularSampler;
        layout(binding = 3) uniform sampler2D u_NormalSampler;
        
        layout(location = 0) in Out
        {
            mat3 tbn;
            vec2 uv;
            vec3 color;

            vec3 fragmentPosition;
            vec3 viewPosition;
        } v_In;
        
        layout(location = 0) out vec4 a_Fragment;
        
        void DirectionalLightContribute(vec3 normal, vec3 viewDirection, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
        {
            vec3 lightDirection = normalize(-directionalLight.direction);
            vec3 reflectDirection = reflect(-lightDirection, normal);
            vec3 halfwayDirection = normalize(lightDirection + viewDirection);

            float diffuseFactor = max(dot(normal, lightDirection), 0.0);
            float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess);

            ambient += directionalLight.ambient;
            diffuse += directionalLight.diffuse * diffuseFactor;
            specular += directionalLight.specular * specularFactor;
        }
        
        void PointLightContribute(vec3 normal, vec3 viewDirection, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
        {          
            int count = pointLights.length();

            for(int i = 0; i < count; i++)
            {
                PointLight light = pointLights[i];

                vec3 positionDifference = (v_In.tbn * light.position) - v_In.fragmentPosition;
                vec3 lightDirection = normalize(positionDifference);
                vec3 reflectDirection = reflect(-lightDirection, normal);
                vec3 halfwayDirection = normalize(lightDirection + viewDirection);

                float distance = length(positionDifference);
                float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

                float diffuseFactor = max(dot(normal, lightDirection), 0.0);
                float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess);

                ambient += light.ambient * attenuation;
                diffuse += light.diffuse * diffuseFactor * attenuation;
                specular += light.specular * specularFactor * attenuation;
            }
        }
        
        vec3 GetLightContribution(vec3 normal, vec3 viewDirection, vec3 ambient, vec3 diffuse, vec3 specular)
        {
            vec3 ambientContribution = vec3(0.0);
            vec3 diffuseContribution = vec3(0.0);
            vec3 specularContribution = vec3(0.0);

            //DirectionalLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);
            PointLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);

            ambientContribution *= ambient;
            diffuseContribution *= diffuse;
            specularContribution *= specular;

            return ambientContribution + diffuseContribution + specularContribution;
        }

        void main()
        {
            vec3 viewDirection = normalize(v_In.viewPosition - v_In.fragmentPosition);

            vec3 materialAlbedo = u_Material.albedo * vec3(texture(u_AlbedoSampler, v_In.uv));
            vec3 materialDiffuse = u_Material.diffuse * vec3(texture(u_DiffuseSampler, v_In.uv));
            vec3 materialSpecular = u_Material.specular * vec3(texture(u_SpecularSampler, v_In.uv));

            vec3 normal = normalize(vec3(texture(u_NormalSampler, v_In.uv)) * 2.0 - 1.0);

            vec3 light = GetLightContribution(normal, viewDirection, materialAlbedo, materialDiffuse, materialSpecular);

            a_Fragment = vec4(v_In.color * light, 1.0);
        }
    )";

    constexpr const char* transparentFragmentShaderSource = R"(
        #version 450

        struct PointLight
        {
            vec3 position;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;

            float constant;
            float linear;
            float quadratic;
        };

        struct DirectionalLight
        {
            vec3 direction;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        layout(std140, binding = 1) uniform Material
        {
            vec3 albedo;
            vec3 diffuse;
            vec3 specular;
            float shininess;
            float opacity;
        } u_Material;  

        layout(std140, binding = 2) uniform DirectionalLights
        {
            DirectionalLight directionalLight;
        };

        layout(std430, binding = 0) buffer PointLights
        {
            PointLight pointLights[];
        };

        layout(binding = 0) uniform sampler2D u_AlbedoSampler;
        layout(binding = 1) uniform sampler2D u_DiffuseSampler;
        layout(binding = 2) uniform sampler2D u_SpecularSampler;
        layout(binding = 3) uniform sampler2D u_NormalSampler;
        layout(binding = 4) uniform sampler2D u_OpacitySampler;
        
        layout(location = 0) in Out
        {
            mat3 tbn;
            vec2 uv;
            vec3 color;

            vec3 fragmentPosition;
            vec3 viewPosition;
        } v_In;
        
        layout(location = 0) out vec4 a_Fragment;
        
        void DirectionalLightContribute(vec3 normal, vec3 viewDirection, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
        {
            vec3 lightDirection = normalize(-directionalLight.direction);
            vec3 reflectDirection = reflect(-lightDirection, normal);
            vec3 halfwayDirection = normalize(lightDirection + viewDirection);

            float diffuseFactor = max(dot(normal, lightDirection), 0.0);
            float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess);

            ambient += directionalLight.ambient;
            diffuse += directionalLight.diffuse * diffuseFactor;
            specular += directionalLight.specular * specularFactor;
        }
        
        void PointLightContribute(vec3 normal, vec3 viewDirection, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
        {          
            int count = pointLights.length();

            for(int i = 0; i < count; i++)
            {
                PointLight light = pointLights[i];

                vec3 positionDifference = (v_In.tbn * light.position) - v_In.fragmentPosition;
                vec3 lightDirection = normalize(positionDifference);
                vec3 reflectDirection = reflect(-lightDirection, normal);
                vec3 halfwayDirection = normalize(lightDirection + viewDirection);

                float distance = length(positionDifference);
                float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

                float diffuseFactor = max(dot(normal, lightDirection), 0.0);
                float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess);

                ambient += light.ambient * attenuation;
                diffuse += light.diffuse * diffuseFactor * attenuation;
                specular += light.specular * specularFactor * attenuation;
            }
        }
        
        vec3 GetLightContribution(vec3 normal, vec3 viewDirection, vec3 ambient, vec3 diffuse, vec3 specular)
        {
            vec3 ambientContribution = vec3(0.0);
            vec3 diffuseContribution = vec3(0.0);
            vec3 specularContribution = vec3(0.0);

            //DirectionalLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);
            PointLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);

            ambientContribution *= ambient;
            diffuseContribution *= diffuse;
            specularContribution *= specular;

            return ambientContribution + diffuseContribution + specularContribution;
        }

        void main()
        {
            vec3 viewDirection = normalize(v_In.viewPosition - v_In.fragmentPosition);

            vec3 materialAlbedo = u_Material.albedo * vec3(texture(u_AlbedoSampler, v_In.uv));
            vec3 materialDiffuse = u_Material.diffuse * vec3(texture(u_DiffuseSampler, v_In.uv));
            vec3 materialSpecular = u_Material.specular * vec3(texture(u_SpecularSampler, v_In.uv));
            float materialOpacity = u_Material.opacity * texture(u_OpacitySampler, v_In.uv).r;

            vec3 normal = normalize(vec3(texture(u_NormalSampler, v_In.uv)) * 2.0 - 1.0);

            vec3 light = GetLightContribution(normal, viewDirection, materialAlbedo, materialDiffuse, materialSpecular);

            a_Fragment = vec4(v_In.color * light, materialOpacity);
        }
    )";

    struct DrawCall 
    {
        const Mesh* mesh = nullptr;
        const Material* material = nullptr;
        glm::mat4 transform = glm::mat4(1.0f);

        DrawCall(const Mesh* mesh, const Material* material, const glm::mat4& transform)
        {
            this->mesh = mesh;
            this->material = material;
            this->transform = transform;
        }
    };

    struct State 
    {
        const Window* window = nullptr;

        std::shared_ptr<RasterPipeline> opaquePipeline = nullptr;
        std::shared_ptr<RasterPipeline> transparentPipeline = nullptr;

        std::shared_ptr<GraphicsBuffer> matrixUniforms = nullptr;
        std::shared_ptr<GraphicsBuffer> materialUniforms = nullptr;
        std::shared_ptr<GraphicsBuffer> directionalLightUniforms = nullptr;

        std::shared_ptr<GraphicsBuffer> lightBuffer = nullptr;

        std::shared_ptr<Sampler> defaultAlbedoSampler = nullptr;
        std::shared_ptr<Sampler> defaultDiffuseSampler = nullptr;
        std::shared_ptr<Sampler> defaultSpecularSampler = nullptr;
        std::shared_ptr<Sampler> defaultNormalSampler = nullptr;
        std::shared_ptr<Sampler> defaultOpacitySampler = nullptr;

        std::vector<PointLight> lights;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);

        glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

        std::vector<DrawCall> opaqueDraws;
        std::vector<DrawCall> transparentDraws;
    } static* state;
    
    void Initialize(const Window& window)
    {
        state = new State;

        state->window = &window;

        state->matrixUniforms = GraphicsBuffer::Create(BufferUsage::Static, (sizeof(glm::mat4) * 4) + sizeof(glm::vec4));
        state->materialUniforms = GraphicsBuffer::Create(BufferUsage::Static, (sizeof(glm::vec4) * 3) + sizeof(glm::vec4));
        state->directionalLightUniforms = GraphicsBuffer::Create(BufferUsage::Static, sizeof(DirectionalLight));

        state->lightBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, 0);

        std::shared_ptr<ShaderModule> vertexShader = ShaderModule::Create(ShaderType::Vertex, vertexShaderSource);

        RasterPipelineDescription opaquePipelineDescription;

        opaquePipelineDescription.UniformBuffers.push_back(state->matrixUniforms);
        opaquePipelineDescription.UniformBuffers.push_back(state->materialUniforms);
        opaquePipelineDescription.UniformBuffers.push_back(state->directionalLightUniforms);

        opaquePipelineDescription.StorageBuffers.push_back(state->lightBuffer);

        opaquePipelineDescription.ShaderModules.push_back(vertexShader);
        opaquePipelineDescription.ShaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, opaqueFragmentShaderSource));

        state->opaquePipeline = RasterPipeline::Create(opaquePipelineDescription);
        
        state->opaquePipeline->SetPolygonFillMode(PolygonFillMode::Solid);
        state->opaquePipeline->SetFaceCullMode(FaceCullMode::Back);
        state->opaquePipeline->SetEnableDepthWriting(true);
        state->opaquePipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);

        RasterPipelineDescription transparentPipelineDescription;

        transparentPipelineDescription.UniformBuffers.push_back(state->matrixUniforms);
        transparentPipelineDescription.UniformBuffers.push_back(state->materialUniforms);
        transparentPipelineDescription.UniformBuffers.push_back(state->directionalLightUniforms);

        transparentPipelineDescription.StorageBuffers.push_back(state->lightBuffer);

        transparentPipelineDescription.ShaderModules.push_back(vertexShader);
        transparentPipelineDescription.ShaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, transparentFragmentShaderSource));

        state->transparentPipeline = RasterPipeline::Create(transparentPipelineDescription);
        
        state->transparentPipeline->SetPolygonFillMode(PolygonFillMode::Solid);
        state->transparentPipeline->SetFaceCullMode(FaceCullMode::None);
        state->transparentPipeline->SetEnableDepthWriting(true);
        state->transparentPipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);
        state->transparentPipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        state->transparentPipeline->SetBlendMode(BlendMode::Add);

        Color32 albedo { 0xFFFFFFFF };
        Color32 diffuse { 0xFFFFFFFF };
        Color32 specular { 0xFFFFFFFF };
        Color32 normal { 128, 128, 255, 0 };
        Color32 opacity { 0xFFFFFFFF };

        std::shared_ptr<Texture> albedoTexture = Texture::Create(TextureType::Texture2D, 1, 1, 1);
        std::shared_ptr<Texture> diffuseTexture = Texture::Create(TextureType::Texture2D, 1, 1, 1);
        std::shared_ptr<Texture> specularTexture = Texture::Create(TextureType::Texture2D, 1, 1, 1);
        std::shared_ptr<Texture> normalTexture = Texture::Create(TextureType::Texture2D, 1, 1, 1);
        std::shared_ptr<Texture> opacityTexture = Texture::Create(TextureType::Texture2D, 1, 1, 1);
        
        albedoTexture->SetData(&albedo);
        diffuseTexture->SetData(&diffuse);
        specularTexture->SetData(&specular);
        normalTexture->SetData(&normal);
        opacityTexture->SetData(&opacity);

        state->defaultAlbedoSampler = Sampler::Create(albedoTexture);

        state->defaultAlbedoSampler->SetMagnification(FilterMode::Nearest);
        state->defaultAlbedoSampler->SetMinification(FilterMode::Nearest);

        state->defaultDiffuseSampler = Sampler::Create(diffuseTexture);
        
        state->defaultDiffuseSampler->SetMagnification(FilterMode::Nearest);
        state->defaultDiffuseSampler->SetMinification(FilterMode::Nearest);

        state->defaultSpecularSampler = Sampler::Create(specularTexture);

        state->defaultSpecularSampler->SetMagnification(FilterMode::Nearest);
        state->defaultSpecularSampler->SetMinification(FilterMode::Nearest);   

        state->defaultNormalSampler = Sampler::Create(normalTexture);
        
        state->defaultNormalSampler->SetMagnification(FilterMode::Nearest);
        state->defaultNormalSampler->SetMinification(FilterMode::Nearest);

        state->defaultOpacitySampler = Sampler::Create(opacityTexture);

        state->defaultOpacitySampler->SetMagnification(FilterMode::Nearest);
        state->defaultOpacitySampler->SetMinification(FilterMode::Nearest);
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

        state->opaqueDraws.clear();
        state->transparentDraws.clear();

        state->projectionMatrix = glm::perspective(
            45.0f * (static_cast<float>(M_PI) / 180.0f),
            static_cast<float>(state->window->GetWidth()) / static_cast<float>(state->window->GetHeight()),
            0.1f, 2000.0f
        );

        state->matrixUniforms->SetData(&state->projectionMatrix, sizeof(glm::mat4), sizeof(glm::mat4) * 2);
    }

    void EndPass()
    {
        DEBUG_ASSERT(state != nullptr);
        
        glm::uvec4 viewport { 0, 0, state->window->GetWidth(), state->window->GetHeight() };

        if(state->opaqueDraws.size() > 0)
        {
            state->opaquePipeline->SetViewport(viewport);

            GraphicsDevice::SetRasterPipeline(state->opaquePipeline.get());
        
            for(const DrawCall& draw : state->opaqueDraws)
            {
                DEBUG_ASSERT(draw.mesh != nullptr && draw.material != nullptr);

                const Mesh& mesh = *draw.mesh;
                const Material& material = *draw.material;
                const glm::mat4& transform = draw.transform;

                const std::shared_ptr<VertexArray>& vertexArray = mesh.GetVertexArray();

                DEBUG_ASSERT(vertexArray != nullptr);

                state->matrixUniforms->SetData(&transform, sizeof(transform));

                float shininess = material.GetShininess();

                state->materialUniforms->SetData(&material.GetAlbedo(), sizeof(material.GetAlbedo()));
                state->materialUniforms->SetData(&material.GetDiffuse(), sizeof(material.GetDiffuse()), sizeof(glm::vec4));        
                state->materialUniforms->SetData(&material.GetSpecular(), sizeof(material.GetSpecular()), sizeof(glm::vec4) * 2);
                state->materialUniforms->SetData(&shininess, sizeof(shininess), sizeof(glm::vec4) * 2 + sizeof(glm::vec3));

                GraphicsDevice::SetVertexArray(vertexArray.get());

                const Sampler* albedo = state->defaultAlbedoSampler.get();
                const Sampler* diffuse = state->defaultDiffuseSampler.get();
                const Sampler* specular = state->defaultSpecularSampler.get();
                const Sampler* normal = state->defaultNormalSampler.get();

                if(material.GetAlbedoSampler())
                {
                    albedo = material.GetAlbedoSampler().get();
                }

                if(material.GetDiffuseSampler())
                {
                    diffuse = material.GetDiffuseSampler().get();
                }

                if(material.GetSpecularSampler())
                {
                    specular = material.GetSpecularSampler().get();
                }

                if(material.GetNormalSampler())
                {
                    normal = material.GetNormalSampler().get();
                }

                GraphicsDevice::BindSampler(albedo, 0);
                GraphicsDevice::BindSampler(diffuse, 1);
                GraphicsDevice::BindSampler(specular, 2);
                GraphicsDevice::BindSampler(normal, 3);
                
                DrawCommand command;

                command.Count = mesh.GetIndexCount();

                GraphicsDevice::DispatchDraw(command);
            }
        }

        if(state->transparentDraws.size() > 0)
        {
            state->transparentPipeline->SetViewport(viewport);

            GraphicsDevice::SetRasterPipeline(state->transparentPipeline.get());

            for(const DrawCall& draw : state->transparentDraws)
            {
                DEBUG_ASSERT(draw.mesh != nullptr && draw.material != nullptr);

                const Mesh& mesh = *draw.mesh;
                const Material& material = *draw.material;
                const glm::mat4& transform = draw.transform;

                const std::shared_ptr<VertexArray>& vertexArray = mesh.GetVertexArray();

                DEBUG_ASSERT(vertexArray != nullptr);

                state->matrixUniforms->SetData(&transform, sizeof(transform));

                float shininess = material.GetShininess();
                float opacity = material.GetOpacity();

                state->materialUniforms->SetData(&material.GetAlbedo(), sizeof(material.GetAlbedo()));
                state->materialUniforms->SetData(&material.GetDiffuse(), sizeof(material.GetDiffuse()), sizeof(glm::vec4));        
                state->materialUniforms->SetData(&material.GetSpecular(), sizeof(material.GetSpecular()), sizeof(glm::vec4) * 2);
                state->materialUniforms->SetData(&shininess, sizeof(shininess), sizeof(glm::vec4) * 2 + sizeof(glm::vec3));
                state->materialUniforms->SetData(&opacity, sizeof(opacity), sizeof(glm::vec4) * 2 + sizeof(glm::vec4));

                GraphicsDevice::SetVertexArray(vertexArray.get());

                const Sampler* albedoSampler = state->defaultAlbedoSampler.get();
                const Sampler* diffuseSampler = state->defaultDiffuseSampler.get();
                const Sampler* specularSampler = state->defaultSpecularSampler.get();
                const Sampler* normalSampler = state->defaultNormalSampler.get();
                const Sampler* opacitySampler = state->defaultOpacitySampler.get();

                if(material.GetAlbedoSampler())
                {
                    albedoSampler = material.GetAlbedoSampler().get();
                }

                if(material.GetDiffuseSampler())
                {
                    diffuseSampler = material.GetDiffuseSampler().get();
                }

                if(material.GetSpecularSampler())
                {
                    specularSampler = material.GetSpecularSampler().get();
                }

                if(material.GetNormalSampler())
                {
                    normalSampler = material.GetNormalSampler().get();
                }

                if(material.GetOpacitySampler())
                {
                    opacitySampler = material.GetOpacitySampler().get();
                }

                GraphicsDevice::BindSampler(albedoSampler, 0);
                GraphicsDevice::BindSampler(diffuseSampler, 1);
                GraphicsDevice::BindSampler(specularSampler, 2);
                GraphicsDevice::BindSampler(normalSampler, 3);
                GraphicsDevice::BindSampler(opacitySampler, 4);
                
                DrawCommand command;

                command.Count = mesh.GetIndexCount();

                GraphicsDevice::DispatchDraw(command);
            }
        }
    }

    void SetDirectionalLight(const DirectionalLight& light)
    {
        DEBUG_ASSERT(state != nullptr);

        state->directionalLightUniforms->SetData(&light, sizeof(light));
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

        if(material.GetOpacity() < 1.0f || material.GetOpacitySampler() != nullptr)
        {
            state->transparentDraws.emplace_back(&mesh, &material, transform);
        }
        else
        {
            state->opaqueDraws.emplace_back(&mesh, &material, transform);   
        }
    }
    
    void DrawModel(const Model& model, const glm::mat4& transform)
    {
        for(const Model::Part& part : model.GetParts())
        {
            DrawMesh(part.mesh, model.GetMaterials()[part.materialIndex], transform);
        }
    }
}