#ifndef ITARGET_RESOURCE_H
#define ITARGET_RESOURCE_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"

namespace Engine {
	class IContext;

	class ITargetResourceData {
	public:
		virtual bool IsDepth() const = 0;
		virtual void Clear(IContext* context) = 0;

		virtual ITextureResourceData* GetTextureResource() const = 0;
	};
}

#endif // !ITARGET_RESOURCE_H
