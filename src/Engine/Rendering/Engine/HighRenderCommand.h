#ifndef HIGH_RENDER_COMMAND_H
#define HIGH_RENDER_COMMAND_H

#include "Engine/Rendering/Engine/VirtualRenderPipeline.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

#define RESOURCE_TAG_GBUFFER_ALBEDO "GBuffer_Albedo"
#define RESOURCE_TAG_GBUFFER_NORMAL "GBuffer_Normal"
#define RESOURCE_TAG_GBUFFER_DEPTH "GBuffer_Depth"
#define RESOURCE_TAG_GBUFFER_ORM "GBuffer_ORM"
#define RESOURCE_TAG_FRAME_HDR "FrameHDR"
#define RESOURCE_TAG_FRAME "Frame"
#define RESOURCE_TAG_DEPTH "Depth"

namespace Engine {
	class IHighRenderCommand {
	public:
		virtual ~IHighRenderCommand() = default;
		virtual void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) = 0;
	};

	class HighRenderCommandPrePass : public IHighRenderCommand {
	public:
		HighRenderCommandPrePass() = default;
		virtual ~HighRenderCommandPrePass() = default;

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;
	};

	class HighRenderCommandBasePass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;

	public:
		HighRenderCommandBasePass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandBasePass();

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;

	private:
		void DrawSingleMesh(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, SceneComponent* component);
	};

	class HighRenderCommandPreLightPass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;

	public:
		HighRenderCommandPreLightPass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandPreLightPass();

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;
	};

	class HighRenderCommandLightPass : public IHighRenderCommand {
	private:
		IShaderResourceData* m_pixelShaderLightSources;
		IShaderResourceData* m_pixelShaderGlobalIllumination;

	public:
		HighRenderCommandLightPass(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandLightPass();

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;

	private:
		void DrawSingleLight(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, SceneComponent* component);
	};

	class HighRenderCommandSkybox : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShader;

	public:
		HighRenderCommandSkybox(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandSkybox();

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;
	};

	class HighRenderCommandPostProcessing : public IHighRenderCommand {
	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShaderGammaCorrection;

	public:
		HighRenderCommandPostProcessing(IRenderResourceFactory* factory);
		virtual ~HighRenderCommandPostProcessing();

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;
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

		void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;

	private:
		void BakeEnvironmentCubemap(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, IBufferResourceData* cubeVertexBuffer, IBufferResourceData* cubeIndexBuffer);
		void BakeIrradianceCubemap(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, IBufferResourceData* cubeVertexBuffer, IBufferResourceData* cubeIndexBuffer);
	};
}

#endif // !HIGH_RENDER_COMMAND_H
