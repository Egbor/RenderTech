#include "Engine/Rendering/Engine/HighRenderContext.h"
#include "Engine/Core/System/Resource/Resource.h"

#include <Windows.h>

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

	constexpr StateData GenerateSkyboxDepthStencilState() noexcept {
		StateData data = InitDefaultStateData<DepthStencilState>();
		data.sdDepthStencil.depthWriteEnable = false;
		data.sdDepthStencil.depthComparisonFunction = ComparisonFunction::CF_LESS_EQUAL;

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

	constexpr StateData GeneratePostProcessingDepthStencilState() noexcept {
		StateData data = InitDefaultStateData<DepthStencilState>();
		data.sdDepthStencil.depthTestEnable = false;
		data.sdDepthStencil.stencilTestEnable = false;

		return data;
	}

	AbstractHighRenderContext::AbstractHighRenderContext(IContext* context)
		: m_context(context) {
		this->OnInitDraw(context);
	}

	AbstractHighRenderContext::~AbstractHighRenderContext() {
		DELETE_ARRAY_OF_OBJECTS(m_commands);
	}

	void AbstractHighRenderContext::ExtendCommandList(AbstractHighRenderCommand* command) {
		m_commands.push_back(command);
	}

	void AbstractHighRenderContext::DrawScene(Scene* scene) {
		HighRenderPipelineAdapter adapter(m_context->QueryPipeline());

		for (Int32 i = 0; i < m_commands.size(); i++) {
			m_commands[i]->Execute(&adapter, scene);
		}
		this->OnPostDraw(m_context);
	}

	HighRenderStorage& AbstractHighRenderContext::GetStorage() {
		return m_storage;
	}

	HRC_Base::HRC_Base(IContext* context) 
		: AbstractHighRenderContext(context) {	
		ExtendCommandList(new HighRenderCommandPrePass(GetStorage()));
		ExtendCommandList(new HighRenderCommandBasePass(GetStorage()));
		ExtendCommandList(new HighRenderCommandLightPass(GetStorage()));
		//ExtendCommandList(new HighRenderCommandSkybox(context->QueryResourceFactory()));
		//ExtendCommandList(new HighRenderCommandPostProcessing(context->QueryResourceFactory()));
	}

	void HRC_Base::OnInitDraw(IContext* context) {
		Viewport viewport;
		context->QueryPipeline()->GetViewport(viewport);

		Int32 width = viewport.GetWidth();
		Int32 height = viewport.GetHeight();

		// Render target preporations
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_GBUFFER_ALBEDO, TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height);
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_GBUFFER_NORMAL, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_GBUFFER_DEPTH, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_GBUFFER_ORM, TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_FRAME_HDR, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_DEPTH, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);
		GetStorage().InitResourceAsTarget(context->QuerySwapChain()->GetOutputTarget(), RESOURCE_TAG_FRAME);

		// Default render states for the base pass
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_BLEND_DEFAULT, StateType::ST_BLEND, InitDefaultStateData<BlendState>());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_SAMPLER_DEFAULT, StateType::ST_SAMPLER, InitDefaultStateData<SamplerState>());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_RESTERIZER_DEFAULT, StateType::ST_RASTERIZER, InitDefaultStateData<RasterizerState>());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_DEPTHSTENCIL_DEFAULT, StateType::ST_DEPTH_STENCIL, InitDefaultStateData<DepthStencilState>());

		// Render states for the first step of the ligh pass stenciling
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_DEPTHSTENCIL_FRONT, StateType::ST_DEPTH_STENCIL, GenerateFrontDepthStencilState());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_RESTERIZER_FRONT, StateType::ST_RASTERIZER, GenerateFronRasterizerState());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_BLEND_FRONT, StateType::ST_BLEND, GenerateFrontBlendState());

		// Render states for the second step of the ligh pass stenciling
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_DEPTHSTENCIL_BACK, StateType::ST_DEPTH_STENCIL, GenerateBackDepthStencilState());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_RESTERIZER_BACK, StateType::ST_RASTERIZER, GenerateBackRasterizerState());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_BLEND_BACK, StateType::ST_BLEND, GenerateBackBlendState());

		// Render states for the skybox pass
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), "SkyboxDepthStencilState", StateType::ST_DEPTH_STENCIL, GenerateSkyboxDepthStencilState());
		// InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_6, "PostProcessingDepthStencilState", StateType::ST_DEPTH_STENCIL, GeneratePostProcessingDepthStencilState());

		GetStorage().InitResourceAsBuffer(context->QueryResourceFactory(), AS_TEXT(UB_ObjectHelper), sizeof(UB_ObjectHelper));
		GetStorage().InitResourceAsBuffer(context->QueryResourceFactory(), AS_TEXT(UB_Object), sizeof(UB_Object));
		GetStorage().InitResourceAsBuffer(context->QueryResourceFactory(), AS_TEXT(UB_Camera), sizeof(UB_Camera));
		GetStorage().InitResourceAsBuffer(context->QueryResourceFactory(), AS_TEXT(UB_Light), sizeof(UB_Light));
	}

	void HRC_Base::OnPostDraw(IContext* context) {
		context->QuerySwapChain()->Swap();
	}

	HRC_IBLBacker::HRC_IBLBacker(IContext* context, const String& filename, Int32 outputWidth, Int32 outputHeight) 
		: AbstractHighRenderContext(context), m_IBLCubeMapOutputWidth(outputWidth), m_IBLCubeMapOutputHeight(outputHeight) {
		m_texture2D = Resource::Load<Texture2D*>(filename);

		ExtendCommandList(new HighRenderCommandBakeHDRIToEnvironmentCubemap(GetStorage()));
	}
	
	HRC_IBLBacker::~HRC_IBLBacker() {
		DELETE_OBJECT(m_texture2D);
	}

	void HRC_IBLBacker::OnInitDraw(IContext* context) {
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_BAKING_ENV_CUBEMAP, TextureType::TT_CUBE, TextureFormat::TF_R32G32B32A32_FLOAT, m_IBLCubeMapOutputWidth, m_IBLCubeMapOutputHeight);
		GetStorage().InitResourceAsTarget(context->QueryResourceFactory(), RESOURCE_TAG_BAKING_IRR_CUBEMAP, TextureType::TT_CUBE, TextureFormat::TF_R32G32B32A32_FLOAT, irrWidth, irrHeight);

		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_SAMPLER_DEFAULT, StateType::ST_SAMPLER, InitDefaultStateData<SamplerState>());
		GetStorage().InitResourceAsState(context->QueryResourceFactory(), RESOURCE_TAG_STATE_RESTERIZER_BACK, StateType::ST_RASTERIZER, GenerateBackRasterizerState());
		GetStorage().InitResourceAsBuffer(context->QueryResourceFactory(), AS_TEXT(UB_Object), sizeof(UB_Object));
	}

	void HRC_IBLBacker::OnPostDraw(IContext* context) {
		if (Resource::Save<ITextureResourceData*>(GetResourceFromBatchByTag<ITextureResourceData>("EnvironmentCubemap"), "assets/textures/skybox/afternoon_env.exr") != ResourceStatus::RS_OK) {
			OutputDebugStringA("[HRC_IBLBacker] Resource::Save() failed for the evironment cubmap");
		}
		if (Resource::Save<ITextureResourceData*>(GetResourceFromBatchByTag<ITextureResourceData>("IrradianceCubemap"), "assets/textures/skybox/afternoon_irr.exr") != ResourceStatus::RS_OK) {
			OutputDebugStringA("[HRC_IBLBacker] Resource::Save() failed for the irradiance cubmap");
		}
	}
}