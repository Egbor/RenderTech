#include "Engine/Rendering/RenderPass/EnviromentMapRenderPass.h"

#include "Engine/Core/System/Import/MeshImport.h"
#include "Engine/Core/System/Import/ShaderImport.h"

namespace Engine {
    EnviromentMapRenderPass::EnviromentMapRenderPass(Context* context, SwapChain* swapchain)
        : RenderPass(context, swapchain), m_buffers(1), m_shaders(2), m_cameraMatrices(6) {
        SetupBuffers();
        SetupShaders();
        SetupCubeMesh();
        SetupCameraMatrices();
    }

    EnviromentMapRenderPass::~EnviromentMapRenderPass() {
        DELETE_OBJECT(m_cubeMesh);
        DELETE_ARRAY_OF_OBJECTS(m_buffers);
        DELETE_ARRAY_OF_OBJECTS(m_shaders);
    }

    void EnviromentMapRenderPass::Render(RenderPass* prev) {
        RasterizerState* stateRasterizer = GetRenderState<RasterizerState>();
        stateRasterizer->SetCullMode(CullMode::D3D11_CULL_FRONT);

        AttachRenderResource(ResourceType::RB_TEXTURE, TextureSlot::TS_ALBEDO, m_inputEquirectangularTexture2D);
        AttachRenderResource(ResourceType::RB_DYNAMIC, RenderDynamicBuffer::RDB_SLOT_0, m_buffers[0]);
        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_VERTEX, m_shaders[0]);
        AttachRenderResource(ResourceType::RB_SHADER, RenderStage::RS_PIXEL, m_shaders[1]);
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_VERTEX, m_cubeMesh->GetSubmesh(0)->GetVertexBuffer());
        AttachRenderResource(ResourceType::RB_BUFFER, RenderBuffer::RB_INDEX, m_cubeMesh->GetSubmesh(0)->GetIndexBuffer());

        BindRenderResources(ResourceType::RB_TEXTURE);
        BindRenderResources(ResourceType::RB_DYNAMIC);
        BindRenderResources(ResourceType::RB_SHADER);
        BindRenderResources(ResourceType::RB_BUFFER);
        BindRenderStates();

        for (Int32 i = 0; i < 6; i++) {
            m_outputCubeTexture2D->SetFaceId(i);

            AddTarget(RenderOutput::RO_TARGET0, m_outputCubeTexture2D);
            UpdateBuffers(m_cameraMatrices[i]);

            AttachRenderResource(ResourceType::RB_TARGET, RenderOutput::RO_TARGET0, GetTarget(RenderOutput::RO_TARGET0));
            BindRenderResources(ResourceType::RB_TARGET);

            GetContext()->DrawIndexed(m_cubeMesh->GetSubmesh(0)->GetNumIndexies());

            UnBindRenderResources(ResourceType::RB_TARGET);
        }

        UnBindRenderResources(ResourceType::RB_TEXTURE);
        UnBindRenderResources(ResourceType::RB_DYNAMIC);
        UnBindRenderResources(ResourceType::RB_SHADER);
        UnBindRenderResources(ResourceType::RB_BUFFER);
        UnBindRenderStates();
    }

    void EnviromentMapRenderPass::SetRenderArguments(Texture2D* srcTexture2D, CubeTexture2D* dstTexture2D) {
        m_inputEquirectangularTexture2D = srcTexture2D;
        m_outputCubeTexture2D = dstTexture2D;
    }

    void EnviromentMapRenderPass::SetupShaders() {
        ShaderImport vertexShaderImport("bin/VSCubeMapGeneration.cso");
        ShaderImport pixelShaderImport("bin/PSCubeMapGeneration.cso");

        ShaderDescription* vertexShaderDesc = vertexShaderImport.LoadResource();
        ShaderDescription* pixelShaderDesc = pixelShaderImport.LoadResource();

        m_shaders[0] = GetContext()->CreateShader(tagREALTIME_SHADER_VERTEX, vertexShaderDesc->GetShaderInfo());
        m_shaders[1] = GetContext()->CreateShader(tagREALTIME_SHADER_FRAGMENT, pixelShaderDesc->GetShaderInfo());

        DELETE_OBJECT(vertexShaderDesc);
        DELETE_OBJECT(pixelShaderDesc);
    }

    void EnviromentMapRenderPass::SetupCubeMesh() {
        MeshImport import("bin/models/cube.obj");
        m_cubeMesh = import.LoadResource();
    }

    void EnviromentMapRenderPass::SetupBuffers() {
        m_buffers[0] = GetContext()->CreateDynamicBuffer(tagREALTIME_BUFFER_VS_CONST_BUFFER, { 0, 1, sizeof(Matrix4x4) });
    }

    void EnviromentMapRenderPass::SetupCameraMatrices() {
        Matrix4x4 proj = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
        m_cameraMatrices[0] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::right, Vector3::up) * proj;
        m_cameraMatrices[1] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::right, Vector3::up) * proj;
        m_cameraMatrices[2] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::up, Vector3::forword) * proj;
        m_cameraMatrices[3] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::up, -Vector3::forword) * proj;
        m_cameraMatrices[4] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::forword, Vector3::up) * proj;
        m_cameraMatrices[5] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::forword, Vector3::up) * proj;
    }

    void EnviromentMapRenderPass::UpdateBuffers(Matrix4x4& matrix) {
        m_buffers[0]->Update({ &matrix, 1, sizeof(matrix) });
    }
}
