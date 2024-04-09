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

	constexpr StateData GenerateFrontDepthStencilState() noexcept {
		StateData data = InitDefaultStateData<DepthStencilState>();
		data.sdDepthStencil.stencilRef = 0;
		data.sdDepthStencil.depthWriteEnable = false;
		data.sdDepthStencil.depthTestEnable = true;
		data.sdDepthStencil.stencilTestEnable = true;
		data.sdDepthStencil.depthComparisonFunction = ComparisonFunction::CF_GREATER;
		data.sdDepthStencil.frontFace.stencilComparisonFunction = ComparisonFunction::CF_ALWAYS;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilDepthFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilPass = StencilOperation::SO_DECR_SAT;
		data.sdDepthStencil.backFace.stencilComparisonFunction = ComparisonFunction::CF_ALWAYS;
		data.sdDepthStencil.backFace.stencilConditions.opStencilFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.backFace.stencilConditions.opStencilDepthFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.backFace.stencilConditions.opStencilPass = StencilOperation::SO_KEEP;
	
		return data;
	}

	constexpr StateData GenerateBackDepthStencilState() noexcept {
		StateData data = InitDefaultStateData<DepthStencilState>();
		data.sdDepthStencil.stencilRef = 1;
		data.sdDepthStencil.depthWriteEnable = false;
		data.sdDepthStencil.depthTestEnable = true;
		data.sdDepthStencil.stencilTestEnable = true;
		data.sdDepthStencil.depthComparisonFunction = ComparisonFunction::CF_GREATER_EQUAL;
		data.sdDepthStencil.backFace.stencilComparisonFunction = ComparisonFunction::CF_EQUAL;
		data.sdDepthStencil.backFace.stencilConditions.opStencilFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.backFace.stencilConditions.opStencilDepthFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.backFace.stencilConditions.opStencilPass = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilComparisonFunction = ComparisonFunction::CF_ALWAYS;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilDepthFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilPass = StencilOperation::SO_KEEP;

		return data;
	}

	constexpr StateData GenerateFronRasterizerState() noexcept {
		StateData data = InitDefaultStateData<RasterizerState>();
		data.sdRasterizer.culling = CullMode::C_BACK;
		data.sdRasterizer.depthClipEnable = true;

		return data;
	}

	constexpr StateData GenerateBackRasterizerState() noexcept {
		StateData data = InitDefaultStateData<RasterizerState>();
		data.sdRasterizer.culling = CullMode::C_FRONT;
		data.sdRasterizer.depthClipEnable = false;

		return data;
	}

	constexpr StateData GenerateFrontBlendState() noexcept {
		StateData data = InitDefaultStateData<BlendState>();
		data.sdBlend.targets[0].blendEnable = false;

		return data;
	}

	constexpr StateData GenerateBackBlendState() noexcept {
		StateData data = InitDefaultStateData<BlendState>();
		data.sdBlend.targets[0].blendEnable = true;
		data.sdBlend.targets[0].blendSrc = Blend::B_ONE;
		data.sdBlend.targets[0].blendDst = Blend::B_ONE;
		data.sdBlend.targets[0].blendOperation = BlendOperation::BO_ADD;

		return data;
	}

	LightRenderPass::LightRenderPass() 
		: m_vertexShader(nullptr), m_pixelShader(nullptr), 
		m_bufferObjectId(), m_bufferObjectHelperId(), m_bufferCameraId(), m_bufferLightId() {

	}

	LightRenderPass::~LightRenderPass() {
		DELETE_OBJECT(m_pixelShader);
		DELETE_OBJECT(m_vertexShader);
	}

	void LightRenderPass::Initialize(ITargetResourceData* output) {
		GetGBuffer().InitNewResource(BatchSlot::BS_SLOT_1, output);

		GetStates().InitNewResource(BatchSlot::BS_SLOT_1, StateType::ST_DEPTH_STENCIL, GenerateFrontDepthStencilState());
		GetStates().InitNewResource(BatchSlot::BS_SLOT_1, StateType::ST_RASTERIZER, GenerateFronRasterizerState());
		GetStates().InitNewResource(BatchSlot::BS_SLOT_1, StateType::ST_BLEND, GenerateFrontBlendState());

		GetStates().InitNewResource(BatchSlot::BS_SLOT_2, StateType::ST_DEPTH_STENCIL, GenerateBackDepthStencilState());
		GetStates().InitNewResource(BatchSlot::BS_SLOT_2, StateType::ST_RASTERIZER, GenerateBackRasterizerState());
		GetStates().InitNewResource(BatchSlot::BS_SLOT_2, StateType::ST_BLEND, GenerateBackBlendState());

		GetUBuffer().InitNewResource(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2, sizeof(UB_Object), &m_bufferObjectId);
		GetUBuffer().InitNewResource(BatchSlot::BS_SLOT_2, sizeof(UB_ObjectHelper), &m_bufferObjectHelperId);
		GetUBuffer().InitNewResource(BatchSlot::BS_SLOT_2, sizeof(UB_Camera), &m_bufferCameraId);
		GetUBuffer().InitNewResource(BatchSlot::BS_SLOT_2, sizeof(UB_Light), &m_bufferLightId);

		m_vertexShader = LoadShader("assets/shaders/LightVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShader = LoadShader("assets/shaders/LightPSShader.cso", ShaderType::ST_PIXEL);
	}

	void LightRenderPass::Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev) {
		IRenderStage* vsStage = pipeline->GetStage(RenderStage::RS_VERTEX);
		IRenderStage* psStage = pipeline->GetStage(RenderStage::RS_PIXEL);

		vsStage->BindShader(m_vertexShader);
		psStage->BindShader(m_pixelShader);
		GetUBuffer().Bind(BatchSlot::BS_SLOT_1, vsStage);
		GetUBuffer().Bind(BatchSlot::BS_SLOT_2, psStage);
		GetGBuffer().Bind(BatchSlot::BS_SLOT_1, pipeline);

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

	void LightRenderPass::SetCamera(Matrix4x4 view, Matrix4x4 proj, Vector3 eyePosition) {
		UB_Object* ubObject = GetUBuffer().GetBufferData(m_bufferObjectId).As<UB_Object>();
		UB_ObjectHelper* ubObjectHelper = GetUBuffer().GetBufferData(m_bufferObjectHelperId).As<UB_ObjectHelper>();
		UB_Camera* ubCamera = GetUBuffer().GetBufferData(m_bufferCameraId).As<UB_Camera>();

		ubObject->ViewProjection = (view * proj).Transpose();
		ubObjectHelper->invProjection = proj.Inverse().Transpose();
		ubObjectHelper->invView = view.Inverse().Transpose();

		ubCamera->Resolution = Vector2(GetRenderWidth(), GetRenderHeight());
		ubCamera->EyePosition = Vector4(eyePosition.x, eyePosition.y, eyePosition.z, 1.0f);
	}
}