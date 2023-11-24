#ifndef IRENDER_PASS_CONTEXT_H
#define IRENDER_PASS_CONTEXT_H

#include "Engine/Rendering/Engine/Interface/IRenderPass.h"
#include "Engine/Object/Component/SceneComponent.h"

namespace Engine {
	class IRenderPassContext {
	public:
		virtual void Append(IRenderPass* pass) = 0;
		virtual void ChangeRenderResolution(Int32 width, Int32 height) = 0;
		virtual void Process(Array<SceneComponent*>& components) = 0;
		virtual void Render() = 0;
	};
}

#endif // !IRENDER_PASS_CONTEXT_H
