#include "Engine/Rendering/RenderPass/SkyboxRenderPass.h"

#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"

#include "Engine/Core/System/Import/MeshImport.h"
#include "Engine/Core/System/Import/TextureImport.h"
#include "Engine/Core/System/Import/ShaderImport.h"

namespace Engine {
    CubeTexture2D* testCubeTexture;
    Mesh* testSkyboxMesh;

    SkyboxRenderPass::SkyboxRenderPass(Context* context, Texture2D* target)
        : ContentRenderPass(context, target), m_buffers(1), m_shaders(2) {

        CubeTextureImport importTexture("bin/texture/cubemap/Forest");
        MeshImport importMesh("bin/models/cube.obj");

        testCubeTexture = importTexture.LoadResource();
        testSkyboxMesh = importMesh.LoadResource();

        SetupBuffers();
        SetupShaders();
        SetupTextures();
    }

    SkyboxRenderPass::~SkyboxRenderPass() {
        DELETE_ARRAY_OF_OBJECTS(m_buffers);
        DELETE_ARRAY_OF_OBJECTS(m_shaders);
        DELETE_OBJECT(testSkyboxMesh);
        DELETE_OBJECT(testCubeTexture);
    }

    void SkyboxRenderPass::Render(RenderPass* prev) {
        ECBStorage& storage = ECBStorage::GetInstance();

        Int32 viewportWidth;
        Int32 viewportHeight;
        GetContext()->GetViewport(viewportWidth, viewportHeight);
        Float aspectRatio = static_cast<Float>(viewportWidth) / static_cast<Float>(viewportHeight);

        CameraComponentBehavior* behaviorCamera = storage.FindComponentBehavior<CameraComponentBehavior>(CameraComponent::TypeIdClass());

        RasterizerState* stateRasterizer = GetRenderState<RasterizerState>();
        DepthStencilState* stateDepthStencil = GetRenderState<DepthStencilState>();

        stateRasterizer->SetCullMode(CullMode::D3D11_CULL_FRONT);
        stateRasterizer->SetDepthClipEnable(false);
        stateDepthStencil->SetDepthTestEnable(true);
        stateDepthStencil->SetDepthWriteEnable(false);
        stateDepthStencil->SetDepthTestComparisonFunction(ComparisonFunction::CF_LESS_EQUAL);

        AddTarget(RenderOutput::RO_TARGET0, GetViewportTarget());
        AddTarget(RenderOutput::RO_DEPTH, prev->GetTarget(RenderOutput::RO_DEPTH));
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET0, GetTarget(RenderOutput::RO_TARGET0));
        AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_DEPTH, GetTarget(RenderOutput::RO_DEPTH));

        BindRenderResources(ResourceType::RB_SHADER);
        BindRenderResources(ResourceType::RB_TARGET);
        BindRenderResources(ResourceType::RB_TEXTURE);
        BindRenderResources(ResourceType::RB_BUFFER);
        BindRenderResources(ResourceType::RB_DYNAMIC);
        BindRenderStates();

        Matrix4x4 matrixWorld = Matrix4x4::CreateMatrixWorld(Vector3::zero, Rotator(0.0f, 0.0f, 0.0f), Vector3(500.0f, 500.0f, 500.0f));
        Matrix4x4 matrixView = behaviorCamera->GetCameraComponentViewMatrix();
        Matrix4x4 matrixProj = behaviorCamera->GetCameraComponentPerspectiveMatrix(aspectRatio);

        UpdateWVPBuffer(matrixWorld, matrixView, matrixProj);
        GetContext()->DrawIndexed(testSkyboxMesh->GetSubmesh(0)->GetNumIndexies());

        UnBindRenderResources(ResourceType::RB_SHADER);
        UnBindRenderResources(ResourceType::RB_TARGET);
        UnBindRenderResources(ResourceType::RB_TEXTURE);
        UnBindRenderResources(ResourceType::RB_BUFFER);
        UnBindRenderResources(ResourceType::RB_DYNAMIC);
        UnBindRenderStates();
    }

    void SkyboxRenderPass::SetupShaders() {
        ShaderImport vertexShaderImport("bin/VSSkyboxShader.cso");
        ShaderImport pixelShaderImport("bin/PSSkyboxShader.cso");

        ShaderDescription* vertexShaderDesc = vertexShaderImport.LoadResource();
        ShaderDescription* pixelShaderDesc = pixelShaderImport.LoadResource();

        m_shaders[0] = GetContext()->CreateShader(tagREALTIME_SHADER_VERTEX, vertexShaderDesc->GetShaderInfo());
        m_shaders[1] = GetContext()->CreateShader(tagREALTIME_SHADER_FRAGMENT, pixelShaderDesc->GetShaderInfo());

        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_VERTEX, m_shaders[0]);
        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_PIXEL, m_shaders[1]);

        DELETE_OBJECT(pixelShaderDesc);
        DELETE_OBJECT(vertexShaderDesc);
    }

    void SkyboxRenderPass::SetupBuffers() {
        m_buffers[0] = GetContext()->CreateDynamicBuffer(tagREALTIME_BUFFER_VS_CONST_BUFFER, { 0, 1, sizeof(Matrix4x4) });

        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_VERTEX, testSkyboxMesh->GetSubmesh(0)->GetVertexBuffer());
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_INDEX, testSkyboxMesh->GetSubmesh(0)->GetIndexBuffer());
        AttachRenderResource(ResourceType::RB_DYNAMIC, RenderDynamicBuffer::RDB_SLOT_0, m_buffers[0]);
    }

    void SkyboxRenderPass::SetupTextures() {
        AttachRenderResource(ResourceType::RB_TEXTURE, 0, testCubeTexture);
    }

    void SkyboxRenderPass::UpdateWVPBuffer(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj) {
        Matrix4x4 wvp = (world * view * proj).Transpose();
        m_buffers[0]->Update({ &wvp, 1, sizeof(Matrix4x4) });
    }
}
