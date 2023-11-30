#ifndef IBUFFER_RESOURCE_H
#define IBUFFER_RESOURCE_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
	class IContext;

	class IBufferResourceData {
	public:
		virtual ~IBufferResourceData() = default;
		virtual Int32 GetNumBytes() const = 0;
		virtual Int32 GetNumElements() const = 0;
	};

	class IDynamicResourceData {
	public:
		virtual ~IDynamicResourceData() = default;
		virtual RawData GetBufferData() = 0;
		virtual void Update(IContext* context) = 0;
	};
}

#endif // !IBUFFER_RESOURCE_H
