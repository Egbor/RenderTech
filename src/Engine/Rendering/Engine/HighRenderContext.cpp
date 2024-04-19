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

	AbstractHighRenderContext::AbstractHighRenderContext(IContext* context)
		: m_pipeline(new VirtualRenderPipeline(context->QuerySwapChain(), context->QueryPipeline())) {

	}

	AbstractHighRenderContext::~AbstractHighRenderContext() {
		DELETE_ARRAY_OF_OBJECTS(m_commands);
	}

	void AbstractHighRenderContext::ExtendCommandList(IHighRenderCommand* command) {
		m_commands.push_back(command);
	}

	void AbstractHighRenderContext::DrawScene(Scene* scene) {
		for (Int32 i = 0; i < m_commands.size(); i++) {
			m_commands[i]->Execute(m_pipeline, scene);
		}
		m_pipeline->SwapBuffers();
	}

	VirtualRenderPipeline* AbstractHighRenderContext::GetPipeline() const {
		return m_pipeline;
	}

	HRC_Base::HRC_Base(IContext* context) 
		: AbstractHighRenderContext(context) {	
		ExtendCommandList(new HighRenderCommandPrePass());
		ExtendCommandList(new HighRenderCommandBasePass(context->QueryResourceFactory()));
		ExtendCommandList(new HighRenderCommandLightPass(context->QueryResourceFactory()));
	}

	void HRC_Base::DrawInit() {
		Int32 width = GetPipeline()->GetRenderSpaceWidth();
		Int32 height = GetPipeline()->GetRenderSpaceHeight();

		GetPipeline()->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height);
		GetPipeline()->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		GetPipeline()->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		GetPipeline()->InitResourceForGBuffer(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);
		GetPipeline()->InitResourceForGBuffer(BatchSlot::BS_SLOT_3);

		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_SAMPLER, InitDefaultStateData<SamplerState>());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_DEPTH_STENCIL, InitDefaultStateData<DepthStencilState>());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_RASTERIZER, InitDefaultStateData<RasterizerState>());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_1, StateType::ST_BLEND, InitDefaultStateData<BlendState>());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_2, StateType::ST_DEPTH_STENCIL, GenerateFrontDepthStencilState());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_2, StateType::ST_RASTERIZER, GenerateFronRasterizerState());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_2, StateType::ST_BLEND, GenerateFrontBlendState());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_3, StateType::ST_DEPTH_STENCIL, GenerateBackDepthStencilState());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_3, StateType::ST_RASTERIZER, GenerateBackRasterizerState());
		GetPipeline()->InitResourceForStates(BatchSlot::BS_SLOT_3, StateType::ST_BLEND, GenerateBackBlendState());

		GetPipeline()->InitResourceForUBuffer(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, AS_TEXT(UB_Object), sizeof(UB_Object));
		GetPipeline()->InitResourceForUBuffer(BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, AS_TEXT(UB_ObjectHelper), sizeof(UB_ObjectHelper));
		GetPipeline()->InitResourceForUBuffer(BatchSlot::BS_SLOT_3, AS_TEXT(UB_Camera), sizeof(UB_Camera));
		GetPipeline()->InitResourceForUBuffer(BatchSlot::BS_SLOT_3, AS_TEXT(UB_Light), sizeof(UB_Light));
	}

	HRC_IBLBacker::HRC_IBLBacker(IContext* context, Int32 outputWidth, Int32 outputHeight) 
		: AbstractHighRenderContext(context), m_IBLCubeMapOutputWidth(outputWidth), m_IBLCubeMapOutputHeight(outputHeight) {
		ExtendCommandList(new HighRenderCommandBakeHDRIToIBL(context->QueryResourceFactory()));
	}

	void HRC_IBLBacker::DrawInit() {
		GetPipeline()->InitResourceForGBuffer(BatchSlot::BS_SLOT_1, TextureType::TT_CUBE, TextureFormat::TF_B8G8R8A8_BMP, m_IBLCubeMapOutputWidth, m_IBLCubeMapOutputHeight);
	}
}