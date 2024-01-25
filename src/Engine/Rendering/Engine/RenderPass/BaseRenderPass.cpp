#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"

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
		GetGBuffer().InitNewResource(RBS_SLOT1,/*TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height*/ output);
		GetGBuffer().InitNewResource(RBS_SLOT1, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		GetGBuffer().InitNewResource(RBS_SLOT1, TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		GetGBuffer().InitNewResource(RBS_SLOT1, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);

		// Samplers initializtion
		GetSamplers().InitNewResource(RBS_SLOT1);

		// Constant buffers initialization
		m_bufferObjectId = GetUBuffer().InitNewResource(RBS_SLOT1, sizeof(UB_Object));
		m_bufferObjectHelperId = GetUBuffer().InitNewResource(RBS_SLOT1, sizeof(UB_ObjectHelper));

		// Default shaders initialization
		m_vertexShader = LoadShader("assets/shaders/BaseVSShader.cso", ShaderType::ST_VERTEX);
	}

	void BaseRenderPass::Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev) {
		IRenderStage* vsStage = pipeline->GetStage(RenderStage::RS_VERTEX);
		IRenderStage* psStage = pipeline->GetStage(RenderStage::RS_PIXEL);

		psStage->BindSamplers(GetSamplers().GetResources(RBS_SLOT1));
		vsStage->BindBuffers(GetUBuffer().GetResources(RBS_SLOT1));
		vsStage->BindShader(m_vertexShader);
		pipeline->SetTargets(GetGBuffer().GetTargets());

		GetGBuffer().Clear();

		for (RenderModel& model = m_queue.back(); !m_queue.empty(); m_queue.pop_back()) {
			UpdateBaseBuffer(model);

			for (Int32 meshElementId = 0; meshElementId < model.mesh->GetNumMeshElements(); meshElementId++) {
				MeshElement* element = model.mesh->GetMeshElement(meshElementId);
				Material* material = model.mesh->GetMaterial(meshElementId);

				psStage->BindTextures(material->GetNativeTextureResources());
				psStage->BindShader(material->GetNativeShaderResource());
				pipeline->Draw(element->GetVertexBuffer(), element->GetIndexBuffer());
			}
		}
	}

	bool BaseRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_BASE;
	}

	RenderModel& BaseRenderPass::ReserveModelInQueue() {
		m_queue.push_back({});
		return m_queue.back();
	}

	void BaseRenderPass::SetCamera(Matrix4x4 view, Matrix4x4 proj) {
		UB_Object* ubObject = GetUBuffer().GetBufferData(m_bufferObjectId).As<UB_Object>();
		ubObject->ViewProjection = (view * proj).Transpose();
	}

	void BaseRenderPass::UpdateBaseBuffer(RenderModel& model) {
		UB_Object* ubObject = GetUBuffer().GetBufferData(m_bufferObjectId).As<UB_Object>();
		UB_ObjectHelper* ubObjectHelper = GetUBuffer().GetBufferData(m_bufferObjectHelperId).As<UB_ObjectHelper>();

		ubObject->World = model.world.Transpose();
		ubObjectHelper->invWorld = model.world.Inverse().Transpose();

		GetUBuffer().Update(m_bufferObjectId);
		GetUBuffer().Update(m_bufferObjectHelperId);
	}
}