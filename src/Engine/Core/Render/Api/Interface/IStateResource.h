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
		CM_NONE		= 0,
		CM_FRONT	= 1,
		CM_BACK		= 2
	};


	class IStateResourceData {
	public:
		virtual ~IStateResourceData() = default;
		virtual void Reset() = 0;
	};

	class AbstractSamplerState : public IStateResourceData {
	public:
		virtual ~AbstractSamplerState() = default;
		virtual void SetAddress(SamplerAddress address) = 0;
		virtual void SetFilter(SamplerFilter filter) = 0;
	};

	class AbstractDepthStencilState : public IStateResourceData {
	public:
		virtual ~AbstractDepthStencilState() = default;

		virtual void SetDepthTestComparisonFunction(ComparisonFunction func) = 0;
		virtual void SetStencilTestComparisonBackFunction(ComparisonFunction func) = 0;
		virtual void SetStencilTestComparisonFrontFunction(ComparisonFunction func) = 0;

		virtual void SetStencilTestBackOperation(StencilConditions conditions) = 0;
		virtual void SetStencilTestFrontOperation(StencilConditions conditions) = 0;

		virtual void SetDepthTestEnable(bool enable) = 0;
		virtual void SetStencilTestEnable(bool enable) = 0;
		virtual void SetDepthWriteEnable(bool enable) = 0;
	};

	class AbstaractRasterizerState : public IStateResourceData {
	public:
		virtual ~AbstaractRasterizerState() = default;

		virtual void SetCullMode(CullMode mode) = 0;
		virtual void SetDepthClipEnable(bool enable) = 0;
	};
}

#endif // !ISTATE_RESOURCE_H
