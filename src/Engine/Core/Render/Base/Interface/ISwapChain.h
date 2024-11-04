#ifndef ISWAPCHAIN_H
#define ISWAPCHAIN_H

#include "Engine/Core/Render/Base/Resource/TargetResource.h"

namespace Engine {
	class ISwapChain {
	public:
		virtual Int32 GetWidth() const = 0;
		virtual Int32 GetHeight() const = 0;

		virtual void Swap() = 0;
		virtual TargetResource* GetOutputTarget() const = 0;
	};
}

#endif // !ISWAPCHAIN_H
