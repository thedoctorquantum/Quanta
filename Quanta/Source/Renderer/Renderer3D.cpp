#include <Quanta/Renderer/Renderer3D.h>
#include <Quanta/Graphics/Pipeline/RasterPipeline.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include "../Debugging/Validation.h"

namespace Quanta::Renderer3D
{
    constexpr const char* vertexShaderSource = R"(
        #version 450

        layout(std140, binding = 0) uniform UniformView
        {
            mat4 model;
            mat4 view;
            mat4 projection;

            mat4 modelViewProjection;
            mat4 viewProjection;

            vec3 position;
            vec3 rotation;
            vec3 target;
        } u_View;

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
            mat3 normalMatrix = transpose(inverse(mat3(u_View.model)));

            vec3 t = normalize(normalMatrix * a_Tangent);
            vec3 n = normalize(normalMatrix * a_Normal);

            t = normalize(t - dot(t, n) * n);

            vec3 b = cross(n, t);

            v_Out.tbn = transpose(mat3(t, b, n));
            v_Out.uv = a_Uv;
            v_Out.color = a_Color;

            vec4 translation = u_View.model * vec4(a_Translation, 1.0);

            v_Out.fragmentPosition = v_Out.tbn * vec3(translation);
            v_Out.viewPosition = v_Out.tbn * u_View.position;

            gl_Position = u_View.viewProjection * translation;
        }
    )";
    
    constexpr const char* opaqueFragmentShaderSource = R"(
        #version 450

        struct PointLight
        {
            vec3 position;
            float intensity;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;

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

        layout(binding = 0) uniform sampler2D u_DiffuseSampler;
        layout(binding = 1) uniform sampler2D u_SpecularSampler;
        layout(binding = 2) uniform sampler2D u_NormalSampler;
        layout(binding = 3) uniform samplerCube u_EnvironmentSampler;
        
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
            float specularFactor = max(dot(normal, halfwayDirection), 0.0);

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
                float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));    
                
                float diffuseFactor = max(dot(normal, lightDirection), 0.0);
                float specularFactor = max(dot(normal, halfwayDirection), 0.0);

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

            DirectionalLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);
            PointLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);

            ambientContribution *= ambient;
            diffuseContribution *= diffuse;
            specularContribution *= pow(specular * float(diffuse != 0.0f), vec3(u_Material.shininess));

            return ambientContribution + diffuseContribution + specularContribution;
        }

        void main()
        {
            vec3 viewDirection = normalize(v_In.viewPosition - v_In.fragmentPosition);

            vec3 diffuse = vec3(texture(u_DiffuseSampler, v_In.uv));
            vec3 specular = vec3(texture(u_SpecularSampler, v_In.uv));

            vec3 materialAlbedo = u_Material.albedo * diffuse;
            vec3 materialDiffuse = u_Material.diffuse * diffuse;
            vec3 materialSpecular = u_Material.specular * specular;

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
            float intensity;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
            
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

        layout(binding = 0) uniform sampler2D u_DiffuseSampler;
        layout(binding = 1) uniform sampler2D u_SpecularSampler;
        layout(binding = 2) uniform sampler2D u_NormalSampler;
        layout(binding = 3) uniform sampler2D u_OpacitySampler;
        
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
                float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));    

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

            DirectionalLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);
            PointLightContribute(normal, viewDirection, ambientContribution, diffuseContribution, specularContribution);

            ambientContribution *= ambient;
            diffuseContribution *= diffuse;
            specularContribution *= specular * float(diffuse != 0.0f);

            return ambientContribution + diffuseContribution + specularContribution;
        }
        
        void main()
        {
            vec3 viewDirection = normalize(v_In.viewPosition - v_In.fragmentPosition);

            vec3 materialAlbedo = u_Material.albedo * vec3(texture(u_DiffuseSampler, v_In.uv));
            vec3 materialDiffuse = u_Material.diffuse * vec3(texture(u_DiffuseSampler, v_In.uv));
            vec3 materialSpecular = u_Material.specular * vec3(texture(u_SpecularSampler, v_In.uv));
            float materialOpacity = u_Material.opacity * texture(u_OpacitySampler, v_In.uv).r;

            vec3 normal = normalize(vec3(texture(u_NormalSampler, v_In.uv)) * 2.0 - 1.0);

            vec3 light = GetLightContribution(normal, viewDirection, materialAlbedo, materialDiffuse, materialSpecular);

            a_Fragment = vec4(v_In.color * light, materialOpacity);
        }
    )";
    
    constexpr const char* environmentVertexShaderSource = R"(
        #version 450

        layout(std140, binding = 0) uniform UniformView
        {
            mat4 model;
            mat4 view;
            mat4 projection;

            mat4 modelViewProjection;
            mat4 viewProjection;

            vec3 position;
            vec3 rotation;
            vec3 target;
        } u_View;

        layout(location = 0) in vec3 a_Translation; 

        layout(location = 0) out Out
        {
            vec3 uvw;
        } v_Out;

        void main()
        {
            v_Out.uvw = a_Translation;

            gl_Position = u_View.projection * mat4(mat3(u_View.view)) * u_View.model * vec4(a_Translation, 1.0);
        }
    )";

    constexpr const char* environmentFragmentShaderSource = R"(
        #version 450 

        layout(location = 0) out vec4 a_Fragment;

        layout(location = 0) in Out
        {
            vec3 uvw;
        } v_In;

        layout(binding = 0) uniform samplerCube u_EnvironmentSampler;

        void main()
        {
            a_Fragment = texture(u_EnvironmentSampler, v_In.uvw);
        }
    )";
    
    struct ShaderView final
    {
    public:
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        glm::mat4 modelViewProjection;
        glm::mat4 viewProjection;

        glm::vec3 position;

        float padding1;

        glm::vec3 rotation;

        float padding2;

        glm::vec3 target;

        float padding3;
    };

    struct ShaderMaterial final
    {
        glm::vec3 albedo;

        float padding1;

        glm::vec3 diffuse;

        float padding2;

        glm::vec3 specular;
        float shininess;
        float opacity;
        
        glm::vec3 padding3;
    };

    struct DrawCall final
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

    struct State final
    {
        const Window* window = nullptr;

        glm::uvec4 viewport = glm::uvec4(0.0f);

        float aspectRatio = 0.0f;

        View view;
        ShaderView shaderView;

        std::shared_ptr<RasterPipeline> opaquePipeline = nullptr;
        std::shared_ptr<RasterPipeline> transparentPipeline = nullptr;
        std::shared_ptr<RasterPipeline> environmentPipeline = nullptr;

        std::shared_ptr<GraphicsBuffer> uniformView = nullptr;
        std::shared_ptr<GraphicsBuffer> uniformMaterial = nullptr;
        std::shared_ptr<GraphicsBuffer> uniformDirectionalLight = nullptr;
        std::shared_ptr<GraphicsBuffer> uniformPointLights = nullptr;

        std::shared_ptr<Sampler> environmentSampler = nullptr;
        std::shared_ptr<Sampler> defaultEnvironmentSampler = nullptr;

        std::shared_ptr<Sampler> defaultAlbedoSampler = nullptr;
        std::shared_ptr<Sampler> defaultSpecularSampler = nullptr;
        std::shared_ptr<Sampler> defaultNormalSampler = nullptr;
        std::shared_ptr<Sampler> defaultOpacitySampler = nullptr;

        std::shared_ptr<FrameBuffer> frameBuffer = nullptr;

        std::shared_ptr<VertexArray> environmentVertexArray = nullptr;

        std::vector<DrawCall> opaqueDraws;
        std::vector<DrawCall> transparentDraws;
    } static* state;
    
    static void OpaquePass();
    static void TransparentPass();
    static void DrawEnvironment();
    
    void Create(const Window& window, const std::shared_ptr<FrameBuffer>& frameBuffer)
    {
        state = new State;

        state->window = &window;

        state->uniformView = GraphicsBuffer::Create(BufferUsage::Static, sizeof(ShaderView));
        state->uniformMaterial = GraphicsBuffer::Create(BufferUsage::Static, sizeof(ShaderMaterial));
        state->uniformDirectionalLight = GraphicsBuffer::Create(BufferUsage::Static, sizeof(DirectionalLight));

        state->uniformPointLights = GraphicsBuffer::Create(BufferUsage::Dynamic, 0);

        const std::shared_ptr<Texture> environmentMap = Texture::Create(Texture::Type::CubeMap, TexelFormat::Rgba8I, 1, 1, 1);

        const Color32 environmentColor { 255, 255, 255, 255 };

        for(std::size_t i = 0; i < 6; i++)
        {
            environmentMap->SetData(&environmentColor, 0, 0, i);
        }

        state->environmentVertexArray = VertexArray::Create();

        const float vertices[]
        {
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f
        };

        const uint8_t indices[] 
        {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            4, 5, 1, 1, 0, 4,
            3, 2, 6, 6, 7, 3
        };

        const std::shared_ptr<GraphicsBuffer> environmentVertexBuffer = GraphicsBuffer::Create(BufferUsage::Static, sizeof(vertices));
        const std::shared_ptr<GraphicsBuffer> environmentIndexBuffer = GraphicsBuffer::Create(BufferUsage::Static, sizeof(indices));

        environmentVertexBuffer->SetData(vertices, sizeof(vertices));
        environmentIndexBuffer->SetData(indices, sizeof(indices));

        VertexLayout layout;

        layout.Add({ 
            BufferPrimitive::Float,
            3,
            sizeof(float),
            false
        });

        state->environmentVertexArray->SetVertexBuffer(environmentVertexBuffer, layout);
        state->environmentVertexArray->SetIndexBuffer(environmentIndexBuffer, IndexType::UInt8);

        state->defaultEnvironmentSampler = Sampler::Create(environmentMap); 

        const std::shared_ptr<ShaderModule> vertexShader = ShaderModule::Create(ShaderType::Vertex, vertexShaderSource);
        
        RasterPipeline::Description opaquePipelineDescription;

        opaquePipelineDescription.uniformBuffers.push_back(state->uniformView);
        opaquePipelineDescription.uniformBuffers.push_back(state->uniformMaterial);
        opaquePipelineDescription.uniformBuffers.push_back(state->uniformDirectionalLight);

        opaquePipelineDescription.storageBuffers.push_back(state->uniformPointLights);

        opaquePipelineDescription.frameBuffer = frameBuffer;

        opaquePipelineDescription.shaderModules.push_back(vertexShader);
        opaquePipelineDescription.shaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, opaqueFragmentShaderSource));

        state->opaquePipeline = RasterPipeline::Create(opaquePipelineDescription);
        
        state->opaquePipeline->SetPolygonFillMode(PolygonFillMode::Solid);
        state->opaquePipeline->SetFaceCullMode(FaceCullMode::Back);
        state->opaquePipeline->SetEnableDepthWriting(true);
        state->opaquePipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);

        RasterPipeline::Description transparentPipelineDescription;

        transparentPipelineDescription.uniformBuffers.push_back(state->uniformView);
        transparentPipelineDescription.uniformBuffers.push_back(state->uniformMaterial);
        transparentPipelineDescription.uniformBuffers.push_back(state->uniformDirectionalLight);

        transparentPipelineDescription.storageBuffers.push_back(state->uniformPointLights);

        transparentPipelineDescription.frameBuffer = frameBuffer;

        transparentPipelineDescription.shaderModules.push_back(vertexShader);
        transparentPipelineDescription.shaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, transparentFragmentShaderSource));

        state->transparentPipeline = RasterPipeline::Create(transparentPipelineDescription);
        
        state->transparentPipeline->SetPolygonFillMode(PolygonFillMode::Solid);
        state->transparentPipeline->SetEnableDepthWriting(true);
        state->transparentPipeline->SetFaceCullMode(FaceCullMode::Back);
        state->transparentPipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);
        state->transparentPipeline->SetBlendFactor(BlendFactor::InverseSourceAlpha);
        state->transparentPipeline->SetBlendMode(BlendMode::Add);

        RasterPipeline::Description environmentPipelineDescription;

        environmentPipelineDescription.uniformBuffers.push_back(state->uniformView);

        environmentPipelineDescription.frameBuffer = frameBuffer;

        environmentPipelineDescription.shaderModules.push_back(ShaderModule::Create(ShaderType::Vertex, environmentVertexShaderSource));
        environmentPipelineDescription.shaderModules.push_back(ShaderModule::Create(ShaderType::Pixel, environmentFragmentShaderSource));

        state->environmentPipeline = RasterPipeline::Create(environmentPipelineDescription);

        state->environmentPipeline->SetFaceCullMode(FaceCullMode::Front);
        state->environmentPipeline->SetEnableDepthWriting(true);
        state->environmentPipeline->SetDepthTestMode(DepthTestMode::LessOrEqual);

        const Color32 albedo { 0xFFFFFFFF };
        const Color32 specular { 0xFFFFFFFF };
        const Color32 normal { 128, 128, 255, 0 };
        const Color32 opacity { 0xFFFFFFFF };

        const std::shared_ptr<Texture> albedoTexture = Texture::Create(Texture::Type::Texture2D, TexelFormat::Rgba8I, 1, 1, 1);
        const std::shared_ptr<Texture> specularTexture = Texture::Create(Texture::Type::Texture2D, TexelFormat::Rgba8I, 1, 1, 1);
        const std::shared_ptr<Texture> normalTexture = Texture::Create(Texture::Type::Texture2D, TexelFormat::Rgba8I, 1, 1, 1);
        const std::shared_ptr<Texture> opacityTexture = Texture::Create(Texture::Type::Texture2D, TexelFormat::Rgba8I, 1, 1, 1);
        
        albedoTexture->SetData(&albedo);
        specularTexture->SetData(&specular);
        normalTexture->SetData(&normal);
        opacityTexture->SetData(&opacity);

        state->defaultAlbedoSampler = Sampler::Create(albedoTexture);

        state->defaultAlbedoSampler->SetMagnification(FilterMode::Nearest);
        state->defaultAlbedoSampler->SetMinification(FilterMode::Nearest);

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
    
    void Destroy()
    {
        DEBUG_ASSERT(state != nullptr);

        delete state;
    }
    
    void BeginPass(const View& view)
    {
        DEBUG_ASSERT(state != nullptr);

        state->aspectRatio = static_cast<float>(view.width) / static_cast<float>(view.height);

        if (state->frameBuffer == nullptr)
        {
            state->viewport = { 0u, 0u, state->window->GetFrameBufferSize().x, state->window->GetFrameBufferSize().y };
        }
        else
        {
            state->viewport = { 0u, 0u, state->frameBuffer->GetWidth(), state->frameBuffer->GetHeight() };
        }
        
        state->view = view;

        state->shaderView.view = view.matrix;
        state->shaderView.projection = glm::perspective(glm::radians(view.fieldOfView), state->aspectRatio, view.near, view.far);

        state->shaderView.viewProjection = state->shaderView.projection * state->shaderView.view;

        state->shaderView.position = { view.matrix[3].x, view.matrix[3].y, view.matrix[3].z };
        state->shaderView.rotation = glm::vec3(0.0f);
        state->shaderView.target = glm::vec3(0.0f);
    }

    void EndPass()
    {
        DEBUG_ASSERT(state != nullptr);

        state->uniformView->SetData(&state->shaderView, sizeof(state->shaderView));

        OpaquePass();   
        DrawEnvironment();     
        TransparentPass();
    } 

    glm::mat4 GetProjectionMatrix()
    {
        return state->shaderView.projection;
    }
    
    glm::mat4 GetViewMatrix()
    {
        return state->shaderView.view;
    }
    
    void SetDirectionalLight(const DirectionalLight& light)
    {
        DEBUG_ASSERT(state != nullptr);

        state->uniformDirectionalLight->SetData(&light, sizeof(light));
    }
    
    void SetPointLights(const PointLight* lights, const std::size_t count)
    {
        DEBUG_ASSERT(lights != nullptr);
        DEBUG_ASSERT(state != nullptr);

        const std::size_t size = count * sizeof(PointLight);

        if(size > state->uniformPointLights->GetSize())
        {
            state->uniformPointLights->Resize(size);
        }

        state->uniformPointLights->SetData(lights, size);
    }
    
    void SetEnvironmentSampler(const std::shared_ptr<Sampler>& value)
    {
        DEBUG_ASSERT(state != nullptr);

        state->environmentSampler = value;
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
            DrawMesh(part.mesh, model.GetMaterials()[part.materialIndex], transform * part.transform);
        }
    }

    static void OpaquePass()
    {
        if(state->opaqueDraws.size() == 0) return;

        state->opaquePipeline->SetViewport(state->viewport);

        GraphicsDevice::SetRasterPipeline(state->opaquePipeline.get());

        for(const DrawCall& draw : state->opaqueDraws)
        {
            DEBUG_ASSERT(draw.mesh != nullptr && draw.material != nullptr);

            const Mesh& mesh = *draw.mesh;
            const Material& material = *draw.material;
            const glm::mat4& transform = draw.transform;

            const std::shared_ptr<VertexArray>& vertexArray = mesh.GetVertexArray();

            DEBUG_ASSERT(vertexArray != nullptr);

            GraphicsDevice::SetVertexArray(vertexArray.get());

            const glm::mat4 modelViewProjection = transform * state->shaderView.viewProjection;

            state->uniformView->SetData(&transform, sizeof(transform), offsetof(ShaderView, ShaderView::model));
            state->uniformView->SetData(&modelViewProjection, sizeof(modelViewProjection), offsetof(ShaderView, ShaderView::modelViewProjection));

            ShaderMaterial materialData;

            materialData.albedo = material.GetAlbedo();
            materialData.diffuse = material.GetDiffuse();
            materialData.specular = material.GetSpecular();
            materialData.shininess = material.GetShininess();
            materialData.opacity = material.GetOpacity();

            state->uniformMaterial->SetData(&materialData, sizeof(materialData));

            const Sampler* albedo = state->defaultAlbedoSampler.get();
            const Sampler* specular = state->defaultSpecularSampler.get();
            const Sampler* normal = state->defaultNormalSampler.get();

            if(material.GetAlbedoSampler())
            {
                albedo = material.GetAlbedoSampler().get();
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
            GraphicsDevice::BindSampler(specular, 1);
            GraphicsDevice::BindSampler(normal, 2);
            
            DrawCommand command;

            command.Count = mesh.GetIndexCount();

            GraphicsDevice::DispatchDraw(command);
        }

        state->opaqueDraws.clear();
    }
    
    static void TransparentPass()
    {
        if(state->transparentDraws.size() == 0) return;

        state->transparentPipeline->SetViewport(state->viewport);

        GraphicsDevice::SetRasterPipeline(state->transparentPipeline.get());

        for(const DrawCall& draw : state->transparentDraws)
        {
            DEBUG_ASSERT(draw.mesh != nullptr && draw.material != nullptr);

            const Mesh& mesh = *draw.mesh;
            const Material& material = *draw.material;
            const glm::mat4& transform = draw.transform;

            const std::shared_ptr<VertexArray>& vertexArray = mesh.GetVertexArray();

            DEBUG_ASSERT(vertexArray != nullptr);

            GraphicsDevice::SetVertexArray(vertexArray.get());

            const glm::mat4 modelViewProjection = transform * state->shaderView.viewProjection;

            state->uniformView->SetData(&transform, sizeof(transform), offsetof(ShaderView, ShaderView::model));
            state->uniformView->SetData(&modelViewProjection, sizeof(modelViewProjection), offsetof(ShaderView, ShaderView::modelViewProjection));

            ShaderMaterial materialData;

            materialData.albedo = material.GetAlbedo();
            materialData.diffuse = material.GetDiffuse();
            materialData.specular = material.GetSpecular();
            materialData.shininess = material.GetShininess();
            materialData.opacity = material.GetOpacity();

            state->uniformMaterial->SetData(&materialData, sizeof(materialData));

            const Sampler* albedoSampler = state->defaultAlbedoSampler.get();
            const Sampler* specularSampler = state->defaultSpecularSampler.get();
            const Sampler* normalSampler = state->defaultNormalSampler.get();
            const Sampler* opacitySampler = state->defaultOpacitySampler.get();

            if(material.GetAlbedoSampler())
            {
                albedoSampler = material.GetAlbedoSampler().get();
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
            GraphicsDevice::BindSampler(specularSampler, 1);
            GraphicsDevice::BindSampler(normalSampler, 2);
            GraphicsDevice::BindSampler(opacitySampler, 3);
            
            DrawCommand command;

            command.Count = mesh.GetIndexCount();

            GraphicsDevice::DispatchDraw(command);
        }

        state->transparentDraws.clear();
    }
    
    static void DrawEnvironment()
    {
        GraphicsDevice::SetRasterPipeline(state->environmentPipeline.get());

        const glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(state->view.far));

        state->uniformView->SetData(&transform, sizeof(transform), offsetof(ShaderView, ShaderView::model));

        GraphicsDevice::SetVertexArray(state->environmentVertexArray.get());

        const Sampler* sampler = state->defaultEnvironmentSampler.get();

        if(state->environmentSampler != nullptr)
        {
            sampler = state->environmentSampler.get();
        }

        GraphicsDevice::BindSampler(sampler, 0); 

        DrawCommand draw;

        draw.Count = state->environmentVertexArray->GetIndexBuffer()->GetSize();

        GraphicsDevice::DispatchDraw(draw);
    }
}