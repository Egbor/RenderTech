#ifndef BUFFER_RESOURCE_H
#define BUFFER_RESOURCE_H

#include "Engine/Core/Render/Base/RenderBase.h"

namespace Engine {
	class BufferResource : public RenderBase {
	public:
		BufferResource(IContext* context) : RenderBase(ResourceIdentifier::RI_BUFFER, context) {}
		virtual ~BufferResource() = default;

		virtual Int32 GetNumberOfBytes() const = 0;
		virtual Int32 GetNumberOfElements() const = 0;
		virtual RawData& GetBufferData() = 0;

		virtual void Update() = 0;

		static ResourceIdentifier GetResourceIdentifier() { return ResourceIdentifier::RI_BUFFER; }
	};
}

#endif // !IBUFFER_RESOURCE_H
