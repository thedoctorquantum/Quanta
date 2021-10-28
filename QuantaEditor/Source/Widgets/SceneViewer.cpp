#include <imgui.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <Quanta/Scene/Components/TransformComponent.h>
#include <Quanta/Scene/Components/ModelRendererComponent.h>
#include <Quanta/Scene/Components/CameraComponent.h>
#include <Quanta/Scene/SceneRenderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "SceneViewer.h"
#include "../Gizmo/ImGuizmo.h"

namespace Quanta
{
    constexpr const char* entityVertexCode = R"(
        #version 450

        layout(location = 0) in vec3 a_Translation;

        layout(std140, binding = 0) uniform Uniforms
        {
            mat4 model;
            mat4 viewProjection;
            int entityId;
        } u_Uniforms;

        void main()
        {
            gl_Position = u_Uniforms.viewProjection * u_Uniforms.model * vec4(a_Translation, 1.0);
        }
    )";
    
    constexpr const char* entityFragmentCode = R"(
        #version 450

        layout(location = 0) out int a_EntityId;

        layout(std140, binding = 0) uniform Uniforms
        {
            mat4 model;
            mat4 viewProjection;
            int entityId;
        } u_Uniforms;

        void main()
        {
            a_EntityId = u_Uniforms.entityId;
        }
    )";

    SceneViewer::SceneViewer(entt::registry& scene, const Window& window) : scene(scene)
    {
        FrameBuffer::Description fbDescription;

        FrameBuffer::AttachmentDescription colorAttachment;

        colorAttachment.format = TexelFormat::Rgba8I;
        colorAttachment.isSwapChainTarget = true;

        FrameBuffer::AttachmentDescription depthAttachment;

        depthAttachment.format = TexelFormat::Depth24Stencil8;

        fbDescription.colorAttachments.push_back(colorAttachment);
        fbDescription.depthAttachment = depthAttachment;

        fbDescription.width = window.GetFrameBufferSize().x;
        fbDescription.height = window.GetFrameBufferSize().y;

        frameBuffer = FrameBuffer::Create(fbDescription);

        frameSampler = Sampler::Create(frameBuffer->GetColorTexture(0));

        FrameBuffer::Description entityFrameBufferDesc;

        FrameBuffer::AttachmentDescription entityAttachment;

        entityAttachment.format = TexelFormat::R32I;

        entityFrameBufferDesc.colorAttachments.push_back(entityAttachment);
        entityFrameBufferDesc.depthAttachment.format = TexelFormat::Depth24Stencil8;
        entityFrameBufferDesc.width = fbDescription.width;
        entityFrameBufferDesc.height = fbDescription.height;

        RasterPipeline::Description entityPipelineDesc;

        entityPipelineDesc.frameBuffer = FrameBuffer::Create(entityFrameBufferDesc);

        entityPipelineDesc.uniformBuffers.push_back(GraphicsBuffer::Create(BufferUsage::Static, sizeof(glm::mat4) * 2 + sizeof(glm::vec4)));

        entityPipelineDesc.vertexShader = ShaderModule::Create(ShaderType::Vertex, entityVertexCode);
        entityPipelineDesc.fragmentShader = ShaderModule::Create(ShaderType::Pixel, entityFragmentCode);

        entityPipeline = RasterPipeline::Create(entityPipelineDesc);

        entityPipeline->depthTestMode = DepthTestMode::LessOrEqual;
        entityPipeline->enableDepthWriting = true;
        entityPipeline->faceCullMode = FaceCullMode::Back;

        view.width = frameBuffer->GetWidth();
        view.height = frameBuffer->GetHeight();

        view.fieldOfView = 70.0f;

        view.matrix = glm::lookAt(glm::vec3 { 0.0f, 10.0f, 10.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, 1.0f, 0.0f });
    }

    void SceneViewer::Render(const char* const title, bool* const open)
    {
        if (!*open)
        {
            return;
        }

        constexpr ImGuiWindowFlags flags = 
            ImGuiWindowFlags_NoScrollbar | 
            ImGuiWindowFlags_NoScrollWithMouse;

        if (ImGui::Begin("Scene", open, flags))
        {
            const std::int32_t clearVal = -1;
            const float clearDepth = 1.0f;

            entityPipeline->framebuffer->Clear({}, 1.0f, 0);

            entityPipeline->framebuffer->GetColorTexture(0)->Clear(&clearVal);

            const auto viewProjection = glm::perspective(
                glm::radians(view.fieldOfView), 
                static_cast<float>(view.width) / static_cast<float>(view.height), 
                view.near, view.far) * view.matrix;

            entityPipeline->uniformBuffers[0]->SetData(&viewProjection, sizeof(viewProjection), sizeof(glm::mat4));

            const auto components = scene.view<TransformComponent, ModelRendererComponent>();

            GraphicsDevice::SetRasterPipeline(entityPipeline.get());

            for (const auto [entity, transform, model] : components.each())
            {
                entityPipeline->uniformBuffers[0]->SetData(&entity, sizeof(entity), sizeof(glm::mat4) * 2);

                for (const auto& part : model.model->parts)
                {
                    GraphicsDevice::SetVertexArray(part.mesh.GetVertexArray().get());

                    const auto localTransform = transform.CreateMatrix();

                    entityPipeline->uniformBuffers[0]->SetData(&localTransform, sizeof(localTransform));

                    DrawCommand command;
                    
                    command.Count = part.mesh.GetIndexCount();

                    GraphicsDevice::DispatchDraw(command);
                }
            }   

            frameBuffer->Clear(glm::vec4(1.0f), 1.0f, 0);

            SceneRenderer::Render(scene, false, view);

            const auto pos = ImGui::GetWindowPos();

            const auto size = ImGui::GetWindowSize();

            view.width = size.x;
            view.height = size.y;

            ImGui::Image(frameSampler.get(), size, ImVec2(0, 0), ImVec2(1, -1));
            
            ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
            ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
            ImGuizmo::AllowAxisFlip(false);

            const auto proj = Renderer3D::GetProjectionMatrix();
            
            if (selectedEntity != entt::null)
            {
                if (scene.try_get<TransformComponent>(selectedEntity))
                {
                    const auto decompose = [](const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale)
                    {
                        pos = m[3];

                        for(int i = 0; i < 3; i++)
                        {
                            scale[i] = glm::length(glm::vec3(m[i]));
                        }

                        const glm::mat3 rotMtx(
                            glm::vec3(m[0]) / scale[0],
                            glm::vec3(m[1]) / scale[1],
                            glm::vec3(m[2]) / scale[2]);
                        rot = glm::quat_cast(rotMtx);
                    };
                    
                    auto& transform = scene.get<TransformComponent>(selectedEntity);

                    ImGuizmo::Enable(ImGui::IsWindowFocused());
                    
                    auto matrix = transform.CreateMatrix();

                    if (ImGuizmo::Manipulate(
                        glm::value_ptr(view.matrix), 
                        glm::value_ptr(proj), 
                        ImGuizmo::OPERATION::UNIVERSAL, 
                        ImGuizmo::MODE::WORLD, 
                        glm::value_ptr(matrix)
                    ))
                    {
                        auto quaternion = glm::identity<glm::quat>();

                        decompose(matrix, transform.translation, quaternion, transform.scale);

                        transform.rotation = glm::degrees(glm::eulerAngles(quaternion));
                    }
                }
            }

            if (!ImGuizmo::IsUsing())
            {
                ImGuizmo::ViewManipulate(glm::value_ptr(view.matrix), 100, ImVec2(pos.x + 20, pos.y + 20), ImVec2(128, 128), 0x00FFFFFF);  
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver() && !ImGuizmo::IsUsing())
            {
                const ImVec2 mousePosRaw { ImGui::GetMousePos().x - ImGui::GetWindowPos().x, ImGui::GetMousePos().y - ImGui::GetWindowPos().y };

                const ImVec2 normalizedWindowPos { mousePosRaw.x / ImGui::GetWindowWidth(), 1.0f - (mousePosRaw.y / ImGui::GetWindowHeight()) }; 

                const glm::uvec2 frameCoordinates
                {
                    static_cast<std::uint32_t>(std::floor(normalizedWindowPos.x * entityPipeline->framebuffer->GetWidth())),
                    static_cast<std::uint32_t>(std::floor(normalizedWindowPos.y * entityPipeline->framebuffer->GetHeight()))
                };

                std::int32_t pixelValue = 0;

                entityPipeline->framebuffer->GetPixel(0, &pixelValue, frameCoordinates.x, frameCoordinates.y);

                selectedEntity = static_cast<entt::entity>(pixelValue);
            }
        }

        ImGui::End();
    }
    
    const std::shared_ptr<FrameBuffer>& SceneViewer::GetFrameBuffer() const
    {
        return frameBuffer;
    }
}