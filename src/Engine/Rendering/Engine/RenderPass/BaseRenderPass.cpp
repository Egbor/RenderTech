#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
	enum class GBuffer_BaseRenderPass {
		GB_BaseColor,
		GB_ViewNormal,
		GB_ORM,
		GB_Depth,
		GB_NUM
	};

	enum class UniformBuffer_BaseRenderPass {
		UB_Camera,
		UB_NUM
	};

	BaseRenderPass::BaseRenderPass(IContext* context, IShaderResourceData* vertexShader)
		: m_gbuffer(static_cast<Size>(GBuffer_BaseRenderPass::GB_NUM)), m_ubuffer(static_cast<Size>(UniformBuffer_BaseRenderPass::UB_NUM))
		, m_vertexShader(vertexShader) {
		IRenderResourceFactory* factory = context->QueryResourceFactory();
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_BaseColor)] = factory->CreateTexture(TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, context->GetWidth(), context->GetHeight());
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_ViewNormal)] = factory->CreateTexture(TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, context->GetWidth(), context->GetHeight());
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_ORM)] = factory->CreateTexture(TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, context->GetWidth(), context->GetHeight());
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_Depth)] = factory->CreateTexture(TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, context->GetWidth(), context->GetHeight());

		UniformBufferCameraDesc cameraUniformBufferDesc;
		m_ubuffer[static_cast<Size>(UniformBuffer_BaseRenderPass::UB_Camera)] = factory->CreateBuffer(BufferType::BT_UNIFORM, 1, sizeof(UniformBufferCameraDesc), &cameraUniformBufferDesc);
	}

	BaseRenderPass::~BaseRenderPass() {
		DELETE_ARRAY_OF_OBJECTS(m_gbuffer);
	}

	void BaseRenderPass::Launch(IRenderPipeline* pipeline) {
		pipeline->SetGBuffer(m_gbuffer);
		pipeline->SetShader(m_vertexShader);
		pipeline->SetBuffer(m_cameraBuffer);

		for (Size i = 0; i < m_modelResourceBatch.size(); i++) {
			BRPModelResource& resource = m_modelResourceBatch[i];
			UniformBufferCameraDesc cameraUniformBufferDesc;

			Matrix4x4 wv = resource.world * m_cameraResource.view;
			Matrix4x4 wvp = wv * m_cameraResource.proj;

			cameraUniformBufferDesc.WorldView = wv.Transpose();
			cameraUniformBufferDesc.invWorldView = wv.Inverse().Transpose();
			cameraUniformBufferDesc.WorldViewProjection = wvp.Transpose();

			pipeline->Update(m_cameraBuffer, 1, &cameraUniformBufferDesc);

			for (Int32 meshElementId = 0; i < resource.meshes->GetNumMeshElements(); i++) {
				MeshElement* element = resource.meshes->GetMeshElement(meshElementId);
				Material* material = resource.meshes->GetMaterial(meshElementId);

				pipeline->SetTextures(material->GetNativeTextureResources());
				pipeline->SetShader(material->GetNativeShaderResource());
				pipeline->Draw(element->GetVertexBuffer(), element->GetIndexBuffer());
			}
		}
	}

	bool BaseRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_BASE;
	}
}