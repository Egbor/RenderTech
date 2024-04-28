#ifndef HIGH_RENDER_COMMAND_H
#define HIGH_RENDER_COMMAND_H

#include "Engine/Rendering/Engine/VirtualRenderPipeline.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

namespace Engine {
	class IHighRenderCommand {
	public:
		virtual ~IHighRenderCommand() = default;
		virtual void Execute(VirtualRenderPipeline* pipeline, Scene* scene) = 0;
	};

	class HighRenderCommandPrePass : public IHighRenderCommand {
	public:
		HighRenderCommandPrePass() = default;
		virtual ~HighRenderCommandPrePass() = default;

		void Execute(VirtualRenderPipeline* pipeline, Scene* scene) override;
	};

	class HighRenderCommandBasePass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;

	public:
		HighRenderCommandBasePass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandBasePass();

		void Execute(VirtualRenderPipeline* pipeline, Scene* scene) override;

	private:
		void DrawSingleMesh(VirtualRenderPipeline* pipeline, SceneComponent* component);
	};

	class HighRenderCommandLightPass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShader;

	public:
		HighRenderCommandLightPass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandLightPass();

		void Execute(VirtualRenderPipeline* pipeline, Scene* scene) override;

	private:
		void DrawSingleLight(VirtualRenderPipeline* pipeline, SceneComponent* component);
	};

	class HighRenderCommandSkybox : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShader;

	public:
		HighRenderCommandSkybox(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandSkybox();

		void Execute(VirtualRenderPipeline* pipeline, Scene* scene) override;
	};

	class HighRenderCommandBakeHDRIToEnvironmentCubemap : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShaderEnvironment;
		IShaderResourceData* m_pixelShaderIrradiance;
		ITextureResourceData* m_equirectangularTexture;

		Array<Matrix4x4> m_mat4x4ViewProjection;

	public:
		HighRenderCommandBakeHDRIToEnvironmentCubemap(IRenderResourceFactory* factory, ITextureResourceData* equirectangularTexture);
		virtual ~HighRenderCommandBakeHDRIToEnvironmentCubemap();

		void Execute(VirtualRenderPipeline* pipeline, Scene* scene) override;

	private:
		void BakeEnvironmentCubemap(VirtualRenderPipeline* pipeline, IBufferResourceData* cubeVertexBuffer, IBufferResourceData* cubeIndexBuffer);
		void BakeIrradianceCubemap(VirtualRenderPipeline* pipeline, IBufferResourceData* cubeVertexBuffer, IBufferResourceData* cubeIndexBuffer);
	};
}

#endif // !HIGH_RENDER_COMMAND_H
