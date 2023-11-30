#ifndef IRENDER_PIPELINE_H
#define IRENDER_PIPELINE_H

#include "Engine/Core/Render/Api/Interface/IRenderStage.h"

namespace Engine {
	class IRenderPipeline {
	public:
		virtual ~IRenderPipeline() = default;
		virtual void Draw(IBufferResourceData* vertex, IBufferResourceData* index) = 0;

		virtual void SetViewport(Int32 width, Int32 height) = 0;
		virtual void SetTargets(const Array<ITargetResourceData*>& targets) = 0;

		virtual void GetViewport(Viewport& viewport) = 0;
		virtual IRenderStage* GetStage(RenderStage stage) = 0;
	};
}

#endif // !IRENDER_PIPELINE_H
