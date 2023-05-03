#include "Engine/Rendering/RenderPass/LightRenderPass.h"

#include "Engine/Object/Component/Behavior/LightComponentBehavior.h"
#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"

#include "Engine/Core/System/Import/ShaderImport.h"

namespace Engine {
    LightRenderPass::LightRenderPass(Context* context, Texture2D* target)
        : ContentRenderPass(context, target), m_buffers(3), m_shaders(2) {
        SetupBuffers();
        SetupShaders();
    }

    LightRenderPass::~LightRenderPass() {
        DELETE_ARRAY_OF_OBJECTS(m_shaders);
        DELETE_ARRAY_OF_OBJECTS(m_buffers);
    }

    void LightRenderPass::Render(RenderPass* prev) {
        ECBStorage& storage = ECBStorage::GetInstance();
        Float aspectRatio = static_cast<Float>(GetViewportTarget()->GetWidth()) / static_cast<Float>(GetViewportTarget()->GetHeight());

        LightComponentBehavior* behaviorLight = storage.FindComponentBehavior<LightComponentBehavior>(LightComponent::TypeIdClass());
        CameraComponentBehavior* behaviorCamera = storage.FindComponentBehavior<CameraComponentBehavior>(CameraComponent::TypeIdClass());

        AddTarget(RenderOutput::RO_TARGET0, GetViewportTarget());
        AddTarget(RenderOutput::RO_DEPTH, prev->GetTarget(RenderOutput::RO_DEPTH));
        ClearOutputTarget();

        behaviorLight->Init();
        while (behaviorLight->HasMore()) {
            Mesh* mesh = behaviorLight->GetCurrentLightVolume();
            Matrix4x4 matrixWorld = behaviorLight->GetCurrentLightWorldMatrix();
            Matrix4x4 matrixView = behaviorCamera->GetCameraComponentViewMatrix();
            Matrix4x4 matrixProj = behaviorCamera->GetCameraComponentPerspectiveMatrix(aspectRatio);
            Vector4 lightValue = behaviorLight->GetCurrentValue();
            Vector4 lightColor = behaviorLight->GetCurrentColor();

            AttachRenderResource(ResourceType::RB_DYNAMIC, RenderDynamicBuffer::RDB_SLOT_0, m_buffers[0]);
            BindRenderResources(ResourceType::RB_DYNAMIC);

            AttachRenderResource(ResourceType::RB_DYNAMIC, RenderDynamicBuffer::RDB_SLOT_0, m_buffers[1]);
            AttachRenderResource(ResourceType::RB_DYNAMIC, RenderDynamicBuffer::RDB_SLOT_1, m_buffers[2]);
            BindRenderResources(ResourceType::RB_DYNAMIC);

            UpdateCameraBuffer(matrixWorld, matrixView, matrixProj);
            UpdateAccumulationBuffer(matrixWorld, matrixView, matrixProj);
            UpdateLightBuffer(lightValue, lightColor);

            StartRenderLightVolume(prev, mesh);
            EndRenderLightVolume(prev, mesh);

            behaviorLight->ToNext();
        }
    }

    void LightRenderPass::SetupBuffers() {
        m_buffers[0] = GetContext()->CreateDynamicBuffer(tagREALTIME_BUFFER_VS_CONST_BUFFER, { 0, 1, sizeof(DynamicBufferCameraDesc) });
        m_buffers[1] = GetContext()->CreateDynamicBuffer(tagREALTIME_BUFFER_PS_CONST_BUFFER, { 0, 1, sizeof(DynamicBufferAccumulationDesc) });
        m_buffers[2] = GetContext()->CreateDynamicBuffer(tagREALTIME_BUFFER_PS_CONST_BUFFER, { 0, 1, sizeof(DynamicBufferLightDesc) });
    }

    void LightRenderPass::SetupShaders() {
        ShaderImport vertexShaderImport("bin/VSLightShader.cso");
        ShaderImport pixelShaderImport("bin/PSLightShader.cso");

        ShaderDescription* vertexShaderDesc = vertexShaderImport.LoadResource();
        ShaderDescription* pixelShaderDesc = pixelShaderImport.LoadResource();

        m_shaders[0] = GetContext()->CreateShader(tagREALTIME_SHADER_VERTEX, vertexShaderDesc->GetShaderInfo());
        m_shaders[1] = GetContext()->CreateShader(tagREALTIME_SHADER_FRAGMENT, pixelShaderDesc->GetShaderInfo());

        DELETE_OBJECT(pixelShaderDesc);
        DELETE_OBJECT(vertexShaderDesc);
    }

    void LightRenderPass::StartRenderLightVolume(RenderPass* prev, Mesh* volume) {
        DepthStencilState* stateDepthStencil = GetRenderState<DepthStencilState>();
        RasterizerState* stateRasterizer = GetRenderState<RasterizerState>();

        stateRasterizer->SetCullMode(CullMode::D3D11_CULL_BACK);
        stateDepthStencil->SetDepthWriteEnable(false);
        stateDepthStencil->SetDepthTestEnable(true);
        stateDepthStencil->SetStencilTestEnable(true);
        stateDepthStencil->SetDepthTestComparisonFunction(ComparisonFunction::CF_GREATER);
        stateDepthStencil->SetStencilTestComparisonBackFunction(ComparisonFunction::CF_ALWAYS);
        stateDepthStencil->SetStencilTestBackOperation({
            StencilOperation::SO_KEEP,
            StencilOperation::SO_KEEP,
            StencilOperation::SO_DECR_SAT
        });

        DetachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET0);
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_DEPTH, prev->GetTarget(RenderOutput::RO_DEPTH));
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_VERTEX, volume->GetSubmesh(0)->GetVertexBuffer());
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_INDEX, volume->GetSubmesh(0)->GetIndexBuffer());
        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_VERTEX, m_shaders[0]);
        DetachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_PIXEL);

        ClearStencilTarget();
        BindRenderResources(ResourceType::RB_TARGET);
        BindRenderResources(ResourceType::RB_BUFFER);
        BindRenderResources(ResourceType::RB_SHADER);
        BindRenderStates();

        GetContext()->DrawIndexed(volume->GetSubmesh(0)->GetNumIndexies());

        UnBindRenderResources(ResourceType::RB_TARGET);
    }

    void LightRenderPass::EndRenderLightVolume(RenderPass* prev, Mesh* volume) {
        BlendState* stateBlend = GetRenderState<BlendState>();
        DepthStencilState* stateDepthStencil = GetRenderState<DepthStencilState>();
        RasterizerState* stateRasterizer = GetRenderState<RasterizerState>();

        stateBlend->SetEnable(RenderOutput::RO_TARGET0, true);
        stateBlend->SetSourceFactor(RenderOutput::RO_TARGET0, Blend::B_ONE);
        stateBlend->SetDestinationFactor(RenderOutput::RO_TARGET0, Blend::B_ONE);
        stateBlend->SetOperation(RenderOutput::RO_TARGET0, BlendOperation::BO_ADD);
        stateRasterizer->SetCullMode(CullMode::D3D11_CULL_FRONT);
        stateRasterizer->SetDepthClipEnable(false);
        stateDepthStencil->SetDepthWriteEnable(false);
        stateDepthStencil->SetDepthTestEnable(true);
        stateDepthStencil->SetStencilTestEnable(true);
        stateDepthStencil->SetStencilRef(1);
        stateDepthStencil->SetDepthTestComparisonFunction(ComparisonFunction::CF_GREATER_EQUAL);
        stateDepthStencil->SetStencilTestComparisonFrontFunction(ComparisonFunction::CF_EQUAL);
        stateDepthStencil->SetStencilTestBackOperation({
            StencilOperation::SO_KEEP,
            StencilOperation::SO_KEEP,
            StencilOperation::SO_KEEP
        });

        Texture2D* albedo = prev->GetTarget(RenderOutput::RO_TARGET0)->GetTexture2D();
        Texture2D* normal = prev->GetTarget(RenderOutput::RO_TARGET1)->GetTexture2D();
        Texture2D* orm = prev->GetTarget(RenderOutput::RO_TARGET2)->GetTexture2D();
        Texture2D* depth = prev->GetTarget(RenderOutput::RO_DEPTH)->GetTexture2D();

        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET0, GetTarget(RenderOutput::RO_TARGET0));
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_DEPTH, GetTarget(RenderOutput::RO_DEPTH));
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_ALBEDO, albedo);
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_NORMAL, normal);
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_DEPTH, depth);
        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_ORM, orm);
        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_PIXEL, m_shaders[1]);

        BindRenderResources(ResourceType::RB_TARGET);
        BindRenderResources(ResourceType::RB_TEXTURE);
        BindRenderResources(ResourceType::RB_BUFFER);
        BindRenderResources(ResourceType::RB_SHADER);
        BindRenderStates();

        GetContext()->DrawIndexed(volume->GetSubmesh(0)->GetNumIndexies());

        UnBindRenderResources(ResourceType::RB_TEXTURE);
        UnBindRenderResources(ResourceType::RB_SHADER);
        UnBindRenderResources(ResourceType::RB_TARGET);
        UnBindRenderStates();
    }

    void LightRenderPass::ClearOutputTarget() {
        TargetClear outputClear;
        memset(outputClear.forTarget.rgba, 0, sizeof(outputClear.forTarget.rgba));
        GetTarget(RenderOutput::RO_TARGET0)->Clear(outputClear);
    }

    void LightRenderPass::ClearStencilTarget() {
        TargetClear stencilClear;
        stencilClear.forDepthStencil.depth = 0.0f;
        stencilClear.forDepthStencil.stencil = 1;
        stencilClear.forDepthStencil.clearDepth = false;
        stencilClear.forDepthStencil.clearStencil = true;
        GetTarget(RenderOutput::RO_DEPTH)->Clear(stencilClear);
    }

    void LightRenderPass::UpdateCameraBuffer(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj) {
        Matrix4x4 wv = world * view;
        Matrix4x4 wvp = wv * proj;

        DynamicBufferCameraDesc cameraBuffer;
        cameraBuffer.WorldView = wv.Transpose();
        cameraBuffer.WorldViewProjection = wvp.Transpose();

        m_buffers[0]->Update({ &cameraBuffer, 1, sizeof(DynamicBufferCameraDesc) });
    }

    void LightRenderPass::UpdateAccumulationBuffer(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj) {
        DynamicBufferAccumulationDesc accumulationBuffer;
        accumulationBuffer.WorldView = (world * view).Transpose();
        accumulationBuffer.Projection = proj.Transpose();
        accumulationBuffer.invProjection = proj.Inverse().Transpose();
        accumulationBuffer.Resolution = Vector2(
            static_cast<Float>(GetViewportTarget()->GetWidth()),
            static_cast<Float>(GetViewportTarget()->GetHeight())
        );

        m_buffers[1]->Update({ &accumulationBuffer, 1, sizeof(DynamicBufferAccumulationDesc) });
    }

    void LightRenderPass::LightRenderPass::UpdateLightBuffer(Vector4& value, Vector4& color) {
        DynamicBufferLightDesc lightBuffer;
        lightBuffer.LightColor = color;
        lightBuffer.LightValue = value;

        m_buffers[2]->Update({ &lightBuffer, 1, sizeof(DynamicBufferLightDesc) });
    }
}
