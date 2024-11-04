#ifndef TEXTURE_RESOURCE_H
#define TEXTURE_RESOURCE_H

#include "Engine/Core/Render/Base/RenderBase.h"

namespace Engine {
	class TextureResource : public RenderBase {
	public:
		TextureResource(IContext* context) : RenderBase(ResourceIdentifier::RI_TEXTURE, context) {}
		virtual ~TextureResource() = default;

		virtual Int32 GetWidth() const = 0;
		virtual Int32 GetHeight() const = 0;
		virtual TextureFormat GetFormat() const = 0;

		virtual bool IsCubemap() const = 0;
		virtual void ReadByCPUAccess(Int8* dstBuffer, Int32 srcSubresource, Size maxSize) const = 0;

		static ResourceIdentifier GetResourceIdentifier() { return ResourceIdentifier::RI_TEXTURE; }
	};
}

#endif // !TEXTURE_RESOURCE_H
