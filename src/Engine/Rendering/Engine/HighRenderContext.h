#ifndef HIGH_RENDER_CONTEXT_H
#define HIGH_RENDER_CONTEXT_H

#include "Engine/Rendering/Engine/HighRenderPipeline.h"
#include "Engine/Rendering/Engine/HighRenderCommand.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

namespace Engine {
	class HighRenderContext {
	private:
		HighRenderPipeline* m_pipeline;
		Array<IHighRenderCommand*> m_commands;

	public:
		HighRenderContext(ITargetResourceData* output, IRenderPipeline* pipeline);
		virtual ~HighRenderContext() = default;

		void DrawScene(Scene* scene);
	};
}

#endif // !HIGH_RENDER_CONTEXT_H
