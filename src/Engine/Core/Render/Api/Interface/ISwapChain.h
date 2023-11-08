#ifndef ISWAPCHAIN_H
#define ISWAPCHAIN_H

#include "Engine/Core/Render/Api/Interface/ITargetResource.h"

namespace Engine {
	class ISwapChain {
	public:
		virtual Int32 GetWidth() const = 0;
		virtual Int32 GetHeight() const = 0;

		virtual void Swap() = 0;
		virtual ITargetResourceData* GetOutputTarget() const = 0;
	};
}

#endif // !ISWAPCHAIN_H
