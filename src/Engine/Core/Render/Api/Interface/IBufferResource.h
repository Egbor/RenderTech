#ifndef IBUFFER_RESOURCE_H
#define IBUFFER_RESOURCE_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
	class IBufferResourceData {
	public:
		virtual Int32 GetNumBytes() const = 0;
		virtual Int32 GetNumElements() const = 0;
	};
}

#endif // !IBUFFER_RESOURCE_H
