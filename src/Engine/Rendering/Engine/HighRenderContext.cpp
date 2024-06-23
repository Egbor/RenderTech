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
		: m_pipeline(new VirtualRenderPipeline(context)) {

	}

	AbstractHighRenderContext::~AbstractHighRenderContext() {
		DELETE_ARRAY_OF_OBJECTS(m_commands);
		DELETE_OBJECT(m_pipeline);
	}

	void AbstractHighRenderContext::ExtendCommandList(IHighRenderCommand* command) {
		m_commands.push_back(command);
	}

	void AbstractHighRenderContext::DrawScene(Scene* scene) {
		this->OnPreDraw();
		for (Int32 i = 0; i < m_commands.size(); i++) {
			m_commands[i]->Execute(m_pipeline, m_storage, scene);
		}
		this->OnPostDraw();
	}

	inline void AbstractHighRenderContext::InitResourceForBatchOfStates(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, StateType type, StateData data) {
		m_storage.InitResourceForBatchOfStates(factory, slots, tag, type, data);
	}

	inline void AbstractHighRenderContext::InitResourceForBatchOfBuffers(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, Int32 bufferSize) {
		m_storage.InitResourceForBatchOfBuffers(factory, slots, tag, bufferSize);
	}

	inline void AbstractHighRenderContext::InitResourceForBatchOfTargets(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		m_storage.InitResourceForBatchOfTargets(factory, slots, tag, type, format, width, height);
	}

	inline void AbstractHighRenderContext::InitResourceForBatchOfTargets(ITargetResourceData* resource, EnumFlags<BatchSlot> slots, const String& tag) {
		m_storage.InitResourceForBatchOfTargets(resource, slots, tag);
	}

	VirtualRenderPipeline* AbstractHighRenderContext::GetPipeline() const {
		return m_pipeline;
	}

	const RenderResourcesStorage& AbstractHighRenderContext::GetStorage() const {
		return m_storage;
	}

	HRC_Base::HRC_Base(IContext* context) 
		: AbstractHighRenderContext(context) {	
		ExtendCommandList(new HighRenderCommandPrePass());
		ExtendCommandList(new HighRenderCommandBasePass(context->QueryResourceFactory()));
		ExtendCommandList(new HighRenderCommandPreLightPass(context->QueryResourceFactory()));
		ExtendCommandList(new HighRenderCommandLightPass(context->QueryResourceFactory()));
		ExtendCommandList(new HighRenderCommandSkybox(context->QueryResourceFactory()));
		ExtendCommandList(new HighRenderCommandPostProcessing(context->QueryResourceFactory()));
	}

	void HRC_Base::DrawInit(IRenderResourceFactory* factory) {
		Int32 width = GetPipeline()->GetRenderSpaceWidth();
		Int32 height = GetPipeline()->GetRenderSpaceHeight();

		// Render target preporations
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_1, RESOURCE_TAG_GBUFFER_ALBEDO, TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height);
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_1, RESOURCE_TAG_GBUFFER_NORMAL, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_1, RESOURCE_TAG_GBUFFER_ORM, TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_4, RESOURCE_TAG_GBUFFER_DEPTH, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, RESOURCE_TAG_DEPTH, TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_3, RESOURCE_TAG_FRAME_HDR, TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		InitResourceForBatchOfTargets(GetPipeline()->GetTarget(), BatchSlot::BS_SLOT_4, RESOURCE_TAG_FRAME);

		// Default render states for the base pass
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_1, "DefaultSamplerState", StateType::ST_SAMPLER, InitDefaultStateData<SamplerState>());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_1, "DefaultDepthStencilState", StateType::ST_DEPTH_STENCIL, InitDefaultStateData<DepthStencilState>());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_5 | BatchSlot::BS_SLOT_6, "DefaultRasterizerState", StateType::ST_RASTERIZER, InitDefaultStateData<RasterizerState>());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_6, "DefaultBlendState", StateType::ST_BLEND, InitDefaultStateData<BlendState>());

		// Render states for the first step of the ligh pass stenciling
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_2, "FrontDepthStencilState", StateType::ST_DEPTH_STENCIL, GenerateFrontDepthStencilState());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_2, "FrontRasterizerState", StateType::ST_RASTERIZER, GenerateFronRasterizerState());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_2, "FrontBlendState", StateType::ST_BLEND, GenerateFrontBlendState());

		// Render states for the second step of the ligh pass stenciling
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_3, "BackDepthStencilState", StateType::ST_DEPTH_STENCIL, GenerateBackDepthStencilState());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_3 | BatchSlot::BS_SLOT_4, "BackRasterizerState", StateType::ST_RASTERIZER, GenerateBackRasterizerState());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_3 | BatchSlot::BS_SLOT_5, "BackBlendState", StateType::ST_BLEND, GenerateBackBlendState());

		// Render states for the skybox pass
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_4 | BatchSlot::BS_SLOT_5 | BatchSlot::BS_SLOT_6, "SkyboxDepthStencilState", StateType::ST_DEPTH_STENCIL, GenerateSkyboxDepthStencilState());
		// InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_6, "PostProcessingDepthStencilState", StateType::ST_DEPTH_STENCIL, GeneratePostProcessingDepthStencilState());

		InitResourceForBatchOfBuffers(factory, BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, AS_TEXT(UB_Object), sizeof(UB_Object));
		InitResourceForBatchOfBuffers(factory, BatchSlot::BS_SLOT_1 | BatchSlot::BS_SLOT_2 | BatchSlot::BS_SLOT_3, AS_TEXT(UB_ObjectHelper), sizeof(UB_ObjectHelper));
		InitResourceForBatchOfBuffers(factory, BatchSlot::BS_SLOT_3, AS_TEXT(UB_Camera), sizeof(UB_Camera));
		InitResourceForBatchOfBuffers(factory, BatchSlot::BS_SLOT_3, AS_TEXT(UB_Light), sizeof(UB_Light));
	}

	void HRC_Base::OnPreDraw() {
		GetPipeline()->AddResources<IBufferResourceData>(GetStorage(), BatchSlot::BS_SLOT_1, RenderStage::RS_VERTEX);
		GetPipeline()->AddResources<IBufferResourceData>(GetStorage(), BatchSlot::BS_SLOT_3, RenderStage::RS_PIXEL);
	}

	void HRC_Base::OnPostDraw() {
		GetPipeline()->SwapBuffers();
	}

	HRC_IBLBacker::HRC_IBLBacker(IContext* context, const String& filename, Int32 outputWidth, Int32 outputHeight) 
		: AbstractHighRenderContext(context), m_IBLCubeMapOutputWidth(outputWidth), m_IBLCubeMapOutputHeight(outputHeight) {
		m_texture2D = Resource::Load<Texture2D*>(filename);

		ExtendCommandList(new HighRenderCommandBakeHDRIToEnvironmentCubemap(context->QueryResourceFactory(), m_texture2D->GetNativeResource()));
	}
	
	HRC_IBLBacker::~HRC_IBLBacker() {
		DELETE_OBJECT(m_texture2D);
	}

	void HRC_IBLBacker::DrawInit(IRenderResourceFactory* factory) {
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_1, "EnvironmentCubemap", TextureType::TT_CUBE, TextureFormat::TF_R32G32B32A32_FLOAT, m_IBLCubeMapOutputWidth, m_IBLCubeMapOutputHeight);
		InitResourceForBatchOfTargets(factory, BatchSlot::BS_SLOT_2, "IrradianceCubemap", TextureType::TT_CUBE, TextureFormat::TF_R32G32B32A32_FLOAT, 32, 32);

		InitResourceForBatchOfBuffers(factory, BatchSlot::BS_SLOT_1, AS_TEXT(UB_Object), sizeof(UB_Object));
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_1, "DefaultSamplerState", StateType::ST_SAMPLER, InitDefaultStateData<SamplerState>());
		InitResourceForBatchOfStates(factory, BatchSlot::BS_SLOT_1, "BackRasterizerState", StateType::ST_RASTERIZER, GenerateBackRasterizerState());
	}

	void HRC_IBLBacker::OnPreDraw() {
		GetPipeline()->AddResources<IBufferResourceData>(GetStorage(), BatchSlot::BS_SLOT_1, RenderStage::RS_VERTEX);
	}

	void HRC_IBLBacker::OnPostDraw() {
		if (Resource::Save<ITextureResourceData*>(GetResourceFromBatchByTag<ITextureResourceData>("EnvironmentCubemap"), "assets/textures/skybox/afternoon_env.exr") != ResourceStatus::RS_OK) {
			OutputDebugStringA("[HRC_IBLBacker] Resource::Save() failed for the evironment cubmap");
		}
		if (Resource::Save<ITextureResourceData*>(GetResourceFromBatchByTag<ITextureResourceData>("IrradianceCubemap"), "assets/textures/skybox/afternoon_irr.exr") != ResourceStatus::RS_OK) {
			OutputDebugStringA("[HRC_IBLBacker] Resource::Save() failed for the irradiance cubmap");
		}
	}
}