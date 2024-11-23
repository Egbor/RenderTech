#ifndef STATE_RESOURCE_H
#define STATE_RESOURCE_H

#include "Engine/Core/Render/Base/RenderBase.h"

namespace Engine {
	enum class ComparisonFunction {
		CF_NEVER			= 0,
		CF_LESS				= 1,
		CF_EQUAL			= 2,
		CF_LESS_EQUAL		= 3,
		CF_GREATER			= 4,
		CF_NOT_EQUAL		= 5,
		CF_GREATER_EQUAL	= 6,
		CF_ALWAYS			= 7
	};

	enum class StencilOperation {
		SO_KEEP		= 0,
		SO_ZERO		= 1,
		SO_REPLACE	= 2,
		SO_INCR_SAT = 3,
		SO_DECR_SAT = 4,
		SO_INVERT	= 5,
		SO_INCR		= 6,
		SO_DECR		= 7
	};

	enum class CullMode {
		C_NONE	= 0,
		C_FRONT	= 1,
		C_BACK	= 2
	};

	enum class Blend {
		B_ZERO				= 0,
		B_ONE				= 1,
		B_SRC_COLOR			= 2,
		B_INV_SRC_COLOR		= 3,
		B_SRC_ALPHA			= 4,
		B_INV_SRC_ALPHA		= 5,
		B_DEST_ALPHA		= 6,
		B_INV_DEST_ALPHA	= 7,
		B_DEST_COLOR		= 8,
		B_INV_DEST_COLOR	= 9,
		B_SRC_ALPHA_SAT		= 10,
		B_BLEND_FACTOR		= 11,
		B_INV_BLEND_FACTOR	= 12,
		B_SRC1_COLOR		= 13,
		B_INV_SRC1_COLOR	= 14,
		B_SRC1_ALPHA		= 15,
		B_INV_SRC1_ALPHA	= 16
	};

	enum class BlendOperation {
		BO_ADD			= 0,
		BO_SUBTRACT		= 1,
		BO_REV_SUBTRACT = 2,
		BO_MIN			= 3,
		BO_MAX			= 4
	};

	struct StencilConditions {
		StencilOperation opStencilFail;
		StencilOperation opStencilDepthFail;
		StencilOperation opStencilPass;
	};

	struct SamplerState {
		SamplerAddress address;
		SamplerFilter filter;
	};

	struct DepthStencilOperations {
		ComparisonFunction stencilComparisonFunction;
		StencilConditions stencilConditions;
	};

	struct DepthStencilState {
		ComparisonFunction depthComparisonFunction;
		DepthStencilOperations backFace;
		DepthStencilOperations frontFace;
		bool depthTestEnable;
		bool depthWriteEnable;
		bool stencilTestEnable;
		Int32 stencilRef;
	};

	struct RasterizerState {
		CullMode culling;
		bool depthClipEnable;
	};

	struct RenderOutputState {
		Blend blendSrc;
		Blend blendDst;
		BlendOperation blendOperation;
		bool blendEnable;
	};

	struct BlendState {
		RenderOutputState targets[8];
	};

	union StateData {
		BlendState sdBlend;
		DepthStencilState sdDepthStencil;
		RasterizerState sdRasterizer;
		SamplerState sdSampler;
	};

	template<class State>
	constexpr StateData InitDefaultStateData() noexcept;

	template<>
	constexpr StateData InitDefaultStateData<SamplerState>() noexcept {
		StateData data {};

		data.sdSampler.address = SamplerAddress::SA_WRAP;
		data.sdSampler.filter = SamplerFilter::SF_LINEAR;

		return data;
	}

	template<>
	constexpr StateData InitDefaultStateData<DepthStencilState>() noexcept {
		StateData data{};

		data.sdDepthStencil.depthTestEnable = true;
		data.sdDepthStencil.depthWriteEnable = true;
		data.sdDepthStencil.stencilTestEnable = false;
		data.sdDepthStencil.depthComparisonFunction = ComparisonFunction::CF_LESS;

		data.sdDepthStencil.backFace.stencilComparisonFunction = ComparisonFunction::CF_ALWAYS;
		data.sdDepthStencil.backFace.stencilConditions.opStencilDepthFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.backFace.stencilConditions.opStencilFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.backFace.stencilConditions.opStencilPass = StencilOperation::SO_KEEP;

		data.sdDepthStencil.frontFace.stencilComparisonFunction = ComparisonFunction::CF_ALWAYS;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilDepthFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilFail = StencilOperation::SO_KEEP;
		data.sdDepthStencil.frontFace.stencilConditions.opStencilPass = StencilOperation::SO_KEEP;

		return data;
	}

	template<>
	constexpr StateData InitDefaultStateData<RasterizerState>() noexcept {
		StateData data {};

		data.sdRasterizer.culling = CullMode::C_BACK;
		data.sdRasterizer.depthClipEnable = true;

		return data;
	}

	template<>
	constexpr StateData InitDefaultStateData<BlendState>() noexcept {
		StateData data {};

		data.sdBlend.targets[0].blendEnable = false;
		data.sdBlend.targets[0].blendSrc = Blend::B_ONE;
		data.sdBlend.targets[0].blendDst = Blend::B_ZERO;
		data.sdBlend.targets[0].blendOperation = BlendOperation::BO_ADD;

		return data;
	}

	class StateResource : public RenderBase {
	public:
		StateResource(const String& name, IContext* context) 
			: RenderBase(name, context, ResourceIdentifier::RI_STATE) {}
		virtual ~StateResource() = default;

		virtual bool Is(StateType type) const = 0;

		static ResourceIdentifier GetResourceIdentifier() { return ResourceIdentifier::RI_STATE; }
	};

	class StandaloneStateResource : public StateResource {
	public:
		StandaloneStateResource(const String& name, IContext* context) : StateResource(name, context) {}
		virtual ~StandaloneStateResource() = default;

		virtual void Bind() const = 0;
	};
}

#endif // !STATE_RESOURCE_H
