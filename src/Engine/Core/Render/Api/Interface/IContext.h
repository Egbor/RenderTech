#ifndef ICONTEXT_H
#define ICONTEXT_H

#include "Engine/Core/Render/Api/Interface/IRenderResourceFactory.h"
#include "Engine/Core/Render/Api/Interface/IRenderPipeline.h"
#include "Engine/Core/Render/Api/Interface/ISwapChain.h"

namespace Engine {
	class IContext {
	public:
		virtual IRenderResourceFactory* QueryResourceFactory() = 0;
		virtual IRenderPipeline* QueryPipeline() = 0;
		virtual ISwapChain* QuerySwapChain() = 0;
	};
}

#endif // !ICONTEXT_H
