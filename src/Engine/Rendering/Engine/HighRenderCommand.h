#ifndef HIGH_RENDER_COMMAND_H
#define HIGH_RENDER_COMMAND_H

#include "Engine/Rendering/Engine/HighRenderPipeline.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

namespace Engine {
	class IHighRenderCommand {
	public:
		virtual ~IHighRenderCommand() = default;
		virtual void Execute(HighRenderPipeline* pipeline, Scene* scene) = 0;
	};

	class HighRenderCommandPrePass : public IHighRenderCommand {
	public:
		HighRenderCommandPrePass() = default;
		virtual ~HighRenderCommandPrePass() = default;

		void Execute(HighRenderPipeline* pipeline, Scene* scene) override;
	};

	class HighRenderCommandBasePass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;

	public:
		HighRenderCommandBasePass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandBasePass();

		void Execute(HighRenderPipeline* pipeline, Scene* scene) override;

	private:
		void DrawSingleMesh(HighRenderPipeline* pipeline, SceneComponent* component);
	};

	class HighRenderCommandLightPass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShader;

	public:
		HighRenderCommandLightPass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandLightPass();

		void Execute(HighRenderPipeline* pipeline, Scene* scene) override;

	private:
		void DrawSingleLight(HighRenderPipeline* pipeline, SceneComponent* component);
	};
}

#endif // !HIGH_RENDER_COMMAND_H
