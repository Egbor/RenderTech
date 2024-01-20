#ifndef ISTATE_RESOURCE_H
#define ISTATE_RESOURCE_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
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
}

#endif // !ISTATE_RESOURCE_H
