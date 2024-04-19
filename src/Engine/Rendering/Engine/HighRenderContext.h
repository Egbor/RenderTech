#ifndef HIGH_RENDER_CONTEXT_H
#define HIGH_RENDER_CONTEXT_H

#include "Engine/Rendering/Engine/VirtualRenderPipeline.h"
#include "Engine/Rendering/Engine/HighRenderCommand.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

namespace Engine {
	class AbstractHighRenderContext {
	private:
		VirtualRenderPipeline* m_pipeline;
		Array<IHighRenderCommand*> m_commands;

	public:
		AbstractHighRenderContext(IContext* context);
		virtual ~AbstractHighRenderContext();

		void ExtendCommandList(IHighRenderCommand* command);
		void DrawScene(Scene* scene);

		virtual void DrawInit() = 0;

	protected:
		VirtualRenderPipeline* GetPipeline() const;
	};

	class HRC_Base : public AbstractHighRenderContext {
	public:
		HRC_Base(IContext* context);
		virtual ~HRC_Base() = default;

		void DrawInit() override;
	};

	class HRC_IBLBacker : public AbstractHighRenderContext {
	private:
		Int32 m_IBLCubeMapOutputWidth;
		Int32 m_IBLCubeMapOutputHeight;

	public:
		HRC_IBLBacker(IContext* context, Int32 outputWidth, Int32 outputHeight);
		virtual ~HRC_IBLBacker() = default;

		void DrawInit() override;
	};
}

#endif // !HIGH_RENDER_CONTEXT_H
