#ifndef HIGH_RENDER_CONTEXT_H
#define HIGH_RENDER_CONTEXT_H

#include "Engine/Rendering/Engine/HighRenderCommand.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

#include "Engine/Object/Class/Texture.h"

namespace Engine {
	class AbstractHighRenderContext {
	private:
		IContext* m_context;
		Array<AbstractHighRenderCommand*> m_commands;

	public:
		AbstractHighRenderContext(IContext* context);
		virtual ~AbstractHighRenderContext();

		void ExtendCommandList(AbstractHighRenderCommand* command);
		void DrawScene(Scene* scene);

	protected:
		HighRenderStorage& GetStorage();

		virtual void OnInitDraw(IContext* context) = 0;
		virtual void OnPostDraw(IContext* context) = 0;

	private:
		HighRenderStorage m_storage;
	};

	class HRC_Base : public AbstractHighRenderContext {
	public:
		HRC_Base(IContext* context);
		virtual ~HRC_Base() = default;

	private:
		void OnInitDraw(IContext* context) override;
		void OnPostDraw(IContext* context) override;
	};

	class HRC_IBLBacker : public AbstractHighRenderContext {
	private:
		static constexpr Int32 irrWidth = 32;
		static constexpr Int32 irrHeight = 32;

		Int32 m_IBLCubeMapOutputWidth;
		Int32 m_IBLCubeMapOutputHeight;

		TargetResource* m_envOutput;
		TargetResource* m_irrOutput;

	public:
		HRC_IBLBacker(IContext* context, const String& filename, Int32 outputWidth, Int32 outputHeight);
		virtual ~HRC_IBLBacker();

	private:
		void OnInitDraw(IContext* context) override;
		void OnPostDraw(IContext* context) override;
	};
}

#endif // !HIGH_RENDER_CONTEXT_H
