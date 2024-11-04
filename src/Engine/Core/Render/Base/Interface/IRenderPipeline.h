#ifndef IRENDER_PIPELINE_H
#define IRENDER_PIPELINE_H

#include "Engine/Core/Render/Base/Interface/IRenderStage.h"
#include "Engine/Core/Render/Base/Resource/TargetResource.h"

namespace Engine {
	class IRenderPipeline {
	public:
		virtual ~IRenderPipeline() = default;
		virtual void Draw(BufferResource* vertex, BufferResource* index) = 0;
		virtual void DrawWaveframe(BufferResource* vertexBuffer, BufferResource* indexBuffer) = 0;

		virtual void SetViewport(Int32 width, Int32 height) = 0;
		virtual void SetTargets(const Array<TargetResource*>& targets) = 0;
		virtual void SetStates(const Array<StateResource*>& states) = 0;

		virtual void GetViewport(Viewport& viewport) = 0;
		virtual IRenderStage* GetStage(RenderStage stage) = 0;
	};
}

#endif // !IRENDER_PIPELINE_H
