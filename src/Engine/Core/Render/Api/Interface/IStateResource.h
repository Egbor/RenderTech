#ifndef ISTATE_RESOURCE_H
#define ISTATE_RESOURCE_H

#include "Engine/Core/Render/Api/RenderDef.h"

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

	struct StencilConditions {
		StencilOperation opStencilFail;
		StencilOperation opStencilDepthFail;
		StencilOperation opStencilPass;
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

	class IContext;

	class IStateData {
	public:
		virtual ~IStateData() = default;
		virtual void Reset() = 0;
	};

	class IStateResourceData {
	public:
		virtual ~IStateResourceData() = default;
		virtual void Initialize(const IStateData* data, const IContext* context) = 0;
	};

	class ISamplerStateData : public IStateData {
	public:
		virtual ~ISamplerStateData() = default;
		virtual void SetAddress(SamplerAddress address) = 0;
		virtual void SetFilter(SamplerFilter filter) = 0;
	};

	class IDepthStencilStateData : public IStateData {
	public:
		virtual ~IDepthStencilStateData() = default;

		virtual void SetDepthTestComparisonFunction(ComparisonFunction func) = 0;
		virtual void SetStencilTestComparisonBackFunction(ComparisonFunction func) = 0;
		virtual void SetStencilTestComparisonFrontFunction(ComparisonFunction func) = 0;

		virtual void SetStencilTestBackOperation(StencilConditions conditions) = 0;
		virtual void SetStencilTestFrontOperation(StencilConditions conditions) = 0;

		virtual void SetDepthTestEnable(bool enable) = 0;
		virtual void SetStencilTestEnable(bool enable) = 0;
		virtual void SetDepthWriteEnable(bool enable) = 0;
	};

	class IRasterizerStateData : public IStateData {
	public:
		virtual ~IRasterizerStateData() = default;

		virtual void SetCullMode(CullMode mode) = 0;
		virtual void SetDepthClipEnable(bool enable) = 0;
	};

	class IBlendStateData : public IStateData {
	public:
		virtual ~IBlendStateData() = default;

		virtual void SetSourceFactor(RenderOutput target, Blend factor) = 0;
		virtual void SetDestinationFactor(RenderOutput target, Blend factor) = 0;
		virtual void SetOperation(RenderOutput target, BlendOperation operation) = 0;

		virtual void SetEnable(RenderOutput target, bool enable) = 0;
	};
}

#endif // !ISTATE_RESOURCE_H
