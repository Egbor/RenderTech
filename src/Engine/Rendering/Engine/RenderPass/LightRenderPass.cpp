#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

namespace Engine {
	LightQueue::LightQueue() 
		: m_queue(), m_dirMark(0) {

	}

	RenderLight& LightQueue::Append(LightType type) {
		Size index = 0;

		if (type == LightType::LT_DIRECTIONAL) {
			m_queue.push_back(m_queue[m_dirMark]);
			index = m_dirMark++;
		} else {
			m_queue.push_back({});
			index = m_queue.size() - 1;
		}

		return m_queue[index];
	}

	void LightQueue::Pop() {
		m_queue.pop_back();
	}

	bool LightQueue::IsEmpty() const {
		return m_queue.empty();
	}

	RenderLight& LightQueue::Top() {
		return m_queue.back();
	}

	LightRenderPass::LightRenderPass() 
		: m_vertexShader(nullptr), m_pixelShader(nullptr) {

	}

	LightRenderPass::~LightRenderPass() {
		DELETE_OBJECT(m_pixelShader);
		DELETE_OBJECT(m_vertexShader);
	}

	void LightRenderPass::Initialize(ITargetResourceData* output) {
		GetGBuffer().InitNewResource(RBS_SLOT1, output);

		m_bufferObjectId = GetUBuffer().InitNewResource(RBS_SLOT1 | RBS_SLOT2, sizeof(UB_Object));
		m_bufferObjectHelperId = GetUBuffer().InitNewResource(RBS_SLOT2, sizeof(UB_ObjectHelper));
		m_bufferSystemId = GetUBuffer().InitNewResource(RBS_SLOT2, sizeof(UB_System));
		m_bufferLightId = GetUBuffer().InitNewResource(RBS_SLOT2, sizeof(UB_Light));

		m_vertexShader = LoadShader("assets/shaders/LightVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShader = LoadShader("assets/shaders/LightPSShader.cso", ShaderType::ST_PIXEL);
	}

	void LightRenderPass::Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev) {
		IRenderStage* vsStage = pipeline->GetStage(RenderStage::RS_VERTEX);
		IRenderStage* psStage = pipeline->GetStage(RenderStage::RS_PIXEL);

		vsStage->BindShader(m_vertexShader);
		psStage->BindShader(m_pixelShader);
		vsStage->BindBuffers(GetUBuffer().GetResources(RBS_SLOT1));
		psStage->BindBuffers(GetUBuffer().GetResources(RBS_SLOT2));
		psStage->BindTextures(prev->GetGBuffer().GetResources(RBS_SLOT_ALL));
		pipeline->SetTargets(GetGBuffer().GetTargets());

		GetGBuffer().Clear();

		for (RenderLight& light = m_lights.Top(); !m_lights.IsEmpty(); m_lights.Pop()) {

		}
	}

	bool LightRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_LIGHT;
	}

	RenderLight& LightRenderPass::ReserveLightInQueue(LightType type) {
		return m_lights.Append(type);
	}

	void LightRenderPass::SetViewProjection(Matrix4x4 view, Matrix4x4 proj) {
		UB_Object* ubObject = GetUBuffer().GetBufferData(m_bufferObjectId).As<UB_Object>();
		UB_ObjectHelper* ubObjectHelper = GetUBuffer().GetBufferData(m_bufferObjectHelperId).As<UB_ObjectHelper>();

		ubObject->ViewProjection = (view * proj).Transpose();
		ubObjectHelper->invProjection = proj.Inverse().Transpose();
		ubObjectHelper->invView = view.Inverse().Transpose();
	}
}