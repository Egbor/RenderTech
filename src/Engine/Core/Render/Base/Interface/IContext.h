#ifndef ICONTEXT_H
#define ICONTEXT_H

#include "Engine/Core/Render/Base/Interface/IRenderResourceFactory.h"
#include "Engine/Core/Render/Base/Interface/IRenderPipeline.h"
#include "Engine/Core/Render/Base/Interface/ISwapChain.h"

namespace Engine {
	class IContext {
	public:
		virtual ~IContext() = default;
		virtual IRenderResourceFactory* QueryResourceFactory() = 0;
		virtual IRenderPipeline* QueryPipeline() = 0;
		virtual ISwapChain* QuerySwapChain() = 0;
	};
}

#endif // !ICONTEXT_H
