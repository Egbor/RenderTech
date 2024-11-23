#ifndef HIGH_RENDER_PIPELINE_H
#define HIGH_RENDER_PIPELINE_H

#include "Engine/Rendering/Engine/RenderBatcher.h"

namespace Engine {
	class HighRenderPipelineAdapter {
	public:
		HighRenderPipelineAdapter(IRenderPipeline* pipeline) noexcept;

		void SetViewportResolution(Int32 width, Int32 height);

		void DisableRenderStage(RenderStage stage);
		void EnableRenderStage(RenderStage stage);

		template<class TResourceClass>
		void BindResources(const Array<TResourceClass*>& resources);
		
		template<class TResourceClass>
		void BindResources(const Array<TResourceClass*>& resources, RenderStage stage);

		IRenderPipeline* GetDirectAccessToPipeline() const;

	private:
		IRenderPipeline* m_pipeline;
	};
}

#endif // !HIGH_RENDER_PIPELINE_H
