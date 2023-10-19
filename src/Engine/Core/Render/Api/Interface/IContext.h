#ifndef ICONTEXT_H
#define ICONTEXT_H

#include "Engine/Core/Render/Api/Interface/IRenderResourceFactory.h"
#include "Engine/Core/Render/Api/Interface/IRenderPipeline.h"

namespace Engine {
	class IContext {
	public:
		virtual Int32 GetWidth() const = 0;
		virtual Int32 GetHeight() const = 0;

		virtual IRenderResourceFactory* QueryResourceFactory() = 0;
		virtual IRenderPipeline* QueryPipeline() = 0;
	};
}

#endif // !ICONTEXT_H
