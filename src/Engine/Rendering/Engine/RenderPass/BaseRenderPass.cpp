#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Core/System/Import/ShaderImport.h"

namespace Engine {
	enum class UBuffer_BaseRenderPass {
		UB_Camera,
		UB_NUM
	};

	BaseRenderPass::BaseRenderPass()
		: m_vertexShader(nullptr) {

	}

	BaseRenderPass::~BaseRenderPass() {
		DELETE_OBJECT(m_vertexShader);
	}

	void BaseRenderPass::Initialize(ITargetResourceData* output) {
		Int32 width = GetRenderWidth();
		Int32 height = GetRenderHeight();

		// GBuffer initialization
		GetGBuffer().Attach(TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height);
		GetGBuffer().Attach(TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		GetGBuffer().Attach(TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		GetGBuffer().Attach(TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);

		// Constant buffers initialization
		GetUBuffer().Attach(sizeof(UniformBufferBase));

		// Default shaders initialization
		m_vertexShader = ShaderLoader::Load("assets/shaders/BaseVSShader.cso", ShaderType::ST_VERTEX);
	}

	void BaseRenderPass::Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev) {
		IRenderStage* vsStage = pipeline->GetStage(RenderStage::RS_VERTEX);
		IRenderStage* psStage = pipeline->GetStage(RenderStage::RS_PIXEL);

		vsStage->BindBuffers(GetUBuffer().GetBuffers());
		vsStage->BindShader(m_vertexShader);
		pipeline->SetTargets(GetGBuffer().GetTargets());

		GetGBuffer().Clear();

		for (; !m_models.empty(); m_models.pop()) {
			Mesh* mesh = UpdateAndGetFrontMesh();

			for (Int32 meshElementId = 0; meshElementId < mesh->GetNumMeshElements(); meshElementId++) {
				MeshElement* element = mesh->GetMeshElement(meshElementId);
				Material* material = mesh->GetMaterial(meshElementId);

				psStage->BindTextures(material->GetNativeTextureResources());
				psStage->BindShader(material->GetNativeShaderResource());
				pipeline->Draw(element->GetVertexBuffer(), element->GetIndexBuffer());
			}
		}
	}

	bool BaseRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_BASE;
	}

	void BaseRenderPass::SetModel(Matrix4x4 world, Mesh* mesh) {
		m_models.push({ world, mesh });
	}

	void BaseRenderPass::SetViewProjection(Matrix4x4 viewproj) {
		UniformBufferBase* desc = GetUBuffer().GetBufferData(INDEX_OF(UBuffer_BaseRenderPass::UB_Camera)).As<UniformBufferBase>();
		desc->ViewProjection = viewproj.Transpose();
	}

	Mesh* BaseRenderPass::UpdateAndGetFrontMesh() {
		UniformBufferBase* desc = GetUBuffer().GetBufferData(INDEX_OF(UBuffer_BaseRenderPass::UB_Camera)).As<UniformBufferBase>();
		desc->World = m_models.front().world.Transpose();
		desc->invWorld = m_models.front().world.Inverse().Transpose();
		GetUBuffer().Update(INDEX_OF(UBuffer_BaseRenderPass::UB_Camera));

		return m_models.front().mesh;
	}
}