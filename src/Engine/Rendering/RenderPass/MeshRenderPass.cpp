#include "Engine/Rendering/RenderPass/MeshRenderPass.h"

#include "Engine/Object/Component/Behavior/MeshComponentBehavior.h"
#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"

#include "Engine/Core/System/Import/ShaderImport.h"

namespace Engine {
    MeshRenderPass::MeshRenderPass(Context* context, SwapChain* swapchain)
        : RenderPass(context, swapchain), m_buffers(1), m_shaders(2) {
        SetupTargets();
        SetupShaders();
        SetupUniform();
    }

    MeshRenderPass::~MeshRenderPass() {
        DELETE_ARRAY_OF_OBJECTS(m_shaders);
        DELETE_ARRAY_OF_OBJECTS(m_buffers);
    }

    void MeshRenderPass::Render(RenderPass* prev) {
        ECBStorage& storage = ECBStorage::GetInstance();
        Float aspectRatio = static_cast<Float>(GetSwapChain()->GetWidth()) / static_cast<Float>(GetSwapChain()->GetHeight());

        MeshComponentBehavior* behaviorMesh = storage.FindComponentBehavior<MeshComponentBehavior>(MeshComponent::TypeIdClass());
        CameraComponentBehavior* behaviorCamera = storage.FindComponentBehavior<CameraComponentBehavior>(CameraComponent::TypeIdClass());

        ClearTargets();
        AttachTargets();

        BindRenderResources(ResourceType::RB_SHADER);
        BindRenderResources(ResourceType::RB_TARGET);
        BindRenderResources(ResourceType::RB_DYNAMIC);
        BindRenderStates();

        behaviorMesh->Init();
        while(behaviorMesh->HasMore()) {
            Mesh* mesh = behaviorMesh->GetCurrentMesh();
            Matrix4x4 matrixWorld = behaviorMesh->GetCurrentMeshWorldMatrix();
            Matrix4x4 matrixView = behaviorCamera->GetCameraComponentViewMatrix();
            Matrix4x4 matrixProj = behaviorCamera->GetCameraComponentPerspectiveMatrix(aspectRatio);

            UpdateUniform(matrixWorld, matrixView, matrixProj);
            for (Int32 i = 0; i< mesh->GetNumSubmeshes(); i++) {
                Submesh* submesh = mesh->GetSubmesh(i);

                UpdateBuffers(submesh);
                UpdateMaterial(submesh->GetMaterial());
                GetContext()->DrawIndexed(submesh->GetNumIndexies());
            }

            behaviorMesh->ToNext();
        }

        UnBindRenderResources(ResourceType::RB_DYNAMIC);
        UnBindRenderResources(ResourceType::RB_TARGET);
        UnBindRenderResources(ResourceType::RB_SHADER);
        UnBindRenderStates();
    }

    void MeshRenderPass::SetupTargets() {
        AddTarget(RenderOutput::RO_TARGET0, TextureFormat::TF_R8G8B8A8_BMP);
        AddTarget(RenderOutput::RO_TARGET1, TextureFormat::TF_R32G32B32A32_FLOAT);
        AddTarget(RenderOutput::RO_TARGET2, TextureFormat::TF_R8G8B8A8_BMP);
        AddTarget(RenderOutput::RO_DEPTH, TextureFormat::TF_R24G8_BMP);
    }

    void MeshRenderPass::AttachTargets() {
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET0, GetTarget(RenderOutput::RO_TARGET0));
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET1, GetTarget(RenderOutput::RO_TARGET1));
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET2, GetTarget(RenderOutput::RO_TARGET2));
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_DEPTH, GetTarget(RenderOutput::RO_DEPTH));
    }

    void MeshRenderPass::ClearTargets() {
        TargetClear outputClear;
        TargetClear depthstencilClear;

        memset(outputClear.forTarget.rgba, 0, sizeof(outputClear.forTarget.rgba));
        depthstencilClear.forDepthStencil.depth = 1.0f;
        depthstencilClear.forDepthStencil.stencil = 0;
        depthstencilClear.forDepthStencil.depth = true;
        depthstencilClear.forDepthStencil.stencil = true;

        GetTarget(RenderOutput::RO_TARGET0)->Clear(outputClear);
        GetTarget(RenderOutput::RO_TARGET1)->Clear(outputClear);
        GetTarget(RenderOutput::RO_TARGET2)->Clear(outputClear);
        GetTarget(RenderOutput::RO_DEPTH)->Clear(depthstencilClear);
    }

    void MeshRenderPass::SetupShaders() {
        ShaderImport vertexShaderImport("bin/VSMeshShader.cso");
        ShaderImport pixelShaderImport("bin/PSMeshShader.cso");

        ShaderDescription* vertexShaderDesc = vertexShaderImport.LoadResource();
        ShaderDescription* pixelShaderDesc = pixelShaderImport.LoadResource();

        m_shaders[0] = GetContext()->CreateShader(tagREALTIME_SHADER_VERTEX, vertexShaderDesc->GetShaderInfo());
        m_shaders[1] = GetContext()->CreateShader(tagREALTIME_SHADER_FRAGMENT, pixelShaderDesc->GetShaderInfo());

        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_VERTEX, m_shaders[0]);
        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_PIXEL, m_shaders[1]);

        DELETE_OBJECT(pixelShaderDesc);
        DELETE_OBJECT(vertexShaderDesc);
    }

    void MeshRenderPass::SetupUniform() {
        m_buffers[0] = GetContext()->CreateDynamicBuffer(tagREALTIME_BUFFER_VS_CONST_BUFFER, { 0, 1, sizeof(DynamicBufferCameraDesc) });
        AttachRenderResource(ResourceType::RB_DYNAMIC, RenderDynamicBuffer::RDB_SLOT_0, m_buffers[0]);
    }

    void MeshRenderPass::UpdateBuffers(Submesh* mesh) {
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_VERTEX, mesh->GetVertexBuffer());
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_INDEX, mesh->GetIndexBuffer());
        BindRenderResources(ResourceType::RB_BUFFER);
    }

    void MeshRenderPass::UpdateMaterial(Material* material) {
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_ALBEDO, material->GetTexture(TextureSlot::TS_ALBEDO));
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_NORMAL, material->GetTexture(TextureSlot::TS_NORMAL));
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_METALLIC, material->GetTexture(TextureSlot::TS_METALLIC));
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_ROUGHNESS, material->GetTexture(TextureSlot::TS_ROUGHNESS));
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_AO, material->GetTexture(TextureSlot::TS_AO));
        BindRenderResources(ResourceType::RB_TEXTURE);
    }

    void MeshRenderPass::UpdateUniform(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj) {
        Matrix4x4 wv = world * view;
        Matrix4x4 wvp = wv * proj;

        DynamicBufferCameraDesc cameraBuffer;
        cameraBuffer.WorldView = wv.Transpose();
        cameraBuffer.WorldViewProjection = wvp.Transpose();

        m_buffers[0]->Update({ &cameraBuffer, 1, sizeof(DynamicBufferCameraDesc) });
    }
}
