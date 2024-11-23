#ifndef TARGET_RESOURCE_H
#define TARGET_RESOURCE_H

#include "Engine/Core/Render/Base/Resource/TextureResource.h"

namespace Engine {
	class TargetResource : public RenderBase {
	public:
		TargetResource(const String& name, IContext* context) 
			: RenderBase(name, context, ResourceIdentifier::RI_TARGET) {}
		virtual ~TargetResource() = default;

		virtual bool IsDepth() const = 0;
		virtual void Copy(TargetResource* dstTarget) const = 0;
		virtual void SelectFace(TextureFace face) = 0;
		virtual void Clear() = 0;

		virtual TextureResource* GetTextureResource() const = 0;

		static ResourceIdentifier GetResourceIdentifier() { return ResourceIdentifier::RI_TARGET; }
	};

	class DepthStencilResource : public TargetResource {
	public:
		DepthStencilResource(const String& name, IContext* context) : TargetResource(name, context) {}
		virtual ~DepthStencilResource() = default;

		virtual void SetStencilClearValue(UInt32 value) = 0;

		virtual void DisableDepthClear() = 0;
		virtual void DisableStencilClear() = 0;

		virtual void EnableDepthClear() = 0;
		virtual void EnableStencilClear() = 0;
	};
}

#endif // !TARGET_RESOURCE_H
