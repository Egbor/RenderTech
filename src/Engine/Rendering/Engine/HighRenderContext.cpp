#include "Engine/Rendering/Engine/HighRenderContext.h"

namespace Engine {
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

	constexpr StateData GenerateDebugDepthStencilState() noexcept {
		StateData data = InitDefaultStateData<DepthStencilState>();
		data.sdDepthStencil.depthWriteEnable = false;
		data.sdDepthStencil.depthTestEnable = false;
		data.sdDepthStencil.stencilTestEnable = false;

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

	constexpr StateData GenerateDebugRasterizerState() noexcept {
		StateData data = InitDefaultStateData<RasterizerState>();
		data.sdRasterizer.culling = CullMode::C_NONE;
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

	HighRenderContext::HighRenderContext(ISwapChain* swapchain, IRenderPipeline* pipeline)
		: m_pipeline(new HighRenderPipeline(swapchain, pipeline)) {
		Int32 width = m_pipeline->GetRenderSpaceWidth();
		Int32 height = m_pipeline->GetRenderSpaceHeight();

		m_pipeline->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height);
		m_pipeline->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		m_pipeline->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		m_pipeline->InitResourceForGBuffer(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);
		m_pipeline->InitResourceForGBuffer(BatchSlot::BS_SLOT_3, swapchain->GetOutputTarget());

		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_SAMPLER, InitDefaultStateData<SamplerState>());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_DEPTH_STENCIL, InitDefaultStateData<DepthStencilState>());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_RASTERIZER, InitDefaultStateData<RasterizerState>());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_BLEND, InitDefaultStateData<BlendState>());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_2, StateType::ST_DEPTH_STENCIL, GenerateFrontDepthStencilState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_2, StateType::ST_RASTERIZER, GenerateFronRasterizerState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_2, StateType::ST_BLEND, GenerateFrontBlendState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_3, StateType::ST_DEPTH_STENCIL, GenerateBackDepthStencilState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_3, StateType::ST_RASTERIZER, GenerateBackRasterizerState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_3, StateType::ST_BLEND, GenerateBackBlendState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_4, StateType::ST_DEPTH_STENCIL, GenerateDebugDepthStencilState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_4, StateType::ST_RASTERIZER, GenerateDebugRasterizerState());
		m_pipeline->InitResourceForStates(BatchSlot::BS_SLOT_4, StateType::ST_BLEND, InitDefaultStateData<BlendState>());

		m_pipeline->InitResourceForUBuffer(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, AS_TEXT(UB_Object), sizeof(UB_Object));
		m_pipeline->InitResourceForUBuffer(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, AS_TEXT(UB_ObjectHelper), sizeof(UB_ObjectHelper));
		m_pipeline->InitResourceForUBuffer(BatchSlot::BS_SLOT_3, AS_TEXT(UB_Camera), sizeof(UB_Camera));
		m_pipeline->InitResourceForUBuffer(BatchSlot::BS_SLOT_3, AS_TEXT(UB_Light), sizeof(UB_Light));
	}

	HighRenderContext::~HighRenderContext() {
		DELETE_ARRAY_OF_OBJECTS(m_commands);
	}

	void HighRenderContext::ExtendCommandList(IHighRenderCommand* command) {
		m_commands.push_back(command);
	}

	void HighRenderContext::DrawScene(Scene* scene) {
		for (Int32 i = 0; i < m_commands.size(); i++) {
			m_commands[i]->Execute(m_pipeline, scene);
		}
		m_pipeline->SwapBuffers();
	}
}