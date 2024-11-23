#ifndef HIGH_RENDER_COMMAND_H
#define HIGH_RENDER_COMMAND_H

#include "Engine/Rendering/Engine/HighRenderPipeline.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

#include "Engine/Object/Component/MeshComponent.h"
#include "Engine/Object/Component/LightComponent.h"

#define RESOURCE_TAG_GBUFFER_ALBEDO "GBuffer_Albedo"
#define RESOURCE_TAG_GBUFFER_NORMAL "GBuffer_Normal"
#define RESOURCE_TAG_GBUFFER_DEPTH "GBuffer_Depth"
#define RESOURCE_TAG_GBUFFER_ORM "GBuffer_ORM"

#define RESOURCE_TAG_STATE_BLEND_DEFAULT "State_DefaultBlend"
#define RESOURCE_TAG_STATE_SAMPLER_DEFAULT "State_DefaultSampler"
#define RESOURCE_TAG_STATE_RESTERIZER_DEFAULT "State_DefaultResterizer"
#define RESOURCE_TAG_STATE_DEPTHSTENCIL_DEFAULT "State_DefaultDepthStencil"

#define RESOURCE_TAG_STATE_BLEND_FRONT "State_FrontBlendState"
#define RESOURCE_TAG_STATE_RESTERIZER_FRONT "State_FrontDepthStencilState"
#define RESOURCE_TAG_STATE_DEPTHSTENCIL_FRONT "State_FrontDepthStencilState"

#define RESOURCE_TAG_STATE_BLEND_BACK "State_BackBlendState"
#define RESOURCE_TAG_STATE_RESTERIZER_BACK "State_BackDepthStencilState"
#define RESOURCE_TAG_STATE_DEPTHSTENCIL_BACK "State_BackDepthStencilState"

#define RESOURCE_TAG_STATE_DEPTHSTENCIL_SKYBOX "SkyboxDepthStencilState"

#define RESOURCE_TAG_BAKING_ENV_CUBEMAP "Cubemap_Environment"
#define RESOURCE_TAG_BAKING_IRR_CUBEMAP "Cubemap_Irradiance"

#define RESOURCE_TAG_FRAME_HDR "FrameHDR"
#define RESOURCE_TAG_FRAME "OutputTarget"
#define RESOURCE_TAG_DEPTH "Depth"

namespace Engine {
	class AbstractHighRenderCommand {
	public:
		AbstractHighRenderCommand();
		virtual ~AbstractHighRenderCommand();

		virtual void Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) = 0;

	protected:
		void ClearTargets(const HighRenderBatcher& batcher);
		void UpdateBuffers(const HighRenderBatcher& batcher);

		virtual void UpdateUBCamera(BufferResource* resource) {};
		virtual void UpdateUBObject(BufferResource* resource) {};
		virtual void UpdateUBHelper(BufferResource* resource) {};
		virtual void UpdateUBLight(BufferResource* resource) {};

	private:
		Map<String, Callable<void(BufferResource*)>*> m_updater;
	};

	class HighRenderCommandPrePass : public AbstractHighRenderCommand {
	public:
		HighRenderCommandPrePass(const HighRenderStorage& storage);
		virtual ~HighRenderCommandPrePass() = default;

		void Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) override;

	private:
		void UpdateUBCamera(BufferResource* resource) override;
		void UpdateUBObject(BufferResource* resource) override;
		void UpdateUBHelper(BufferResource* resource) override;

		Matrix4x4 m_view;
		Matrix4x4 m_proj;

		Viewport m_currentResolution;
		CameraComponent* m_camera;

		HighRenderBatcher m_batcher;
	};

	class HighRenderCommandBasePass : public AbstractHighRenderCommand {
	public:
		HighRenderCommandBasePass(const HighRenderStorage& storage);
		virtual ~HighRenderCommandBasePass() = default;

		void Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) override;

	private:
		void DrawSingleMesh(HighRenderPipelineAdapter* pipeline);
		void UpdateUBObject(BufferResource* resource) override;
		void UpdateUBHelper(BufferResource* resource) override;

		SceneComponent* m_component;

		HighRenderBatcher m_batcher;
	};

	class HighRenderCommandLightPass : public AbstractHighRenderCommand {
	public:
		HighRenderCommandLightPass(const HighRenderStorage& storage);
		virtual ~HighRenderCommandLightPass() = default;

		void Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) override;

	private:
		void DrawSingleLight(HighRenderPipelineAdapter* pipeline);
		void UpdateUBObject(BufferResource* resource) override;
		void UpdateUBHelper(BufferResource* resource) override;
		void UpdateUBLight(BufferResource* resource) override;

		LightComponent* m_component;

		HighRenderBatcher m_batcher;
	};

	//class HighRenderCommandSkybox : public AbstractHighRenderCommand {
	//public:
	//	HighRenderCommandSkybox(const HighRenderStorage& storage);
	//	virtual ~HighRenderCommandSkybox() = default;

	//	void Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) override;

	//private:
	//	void UpdateUBObject(IDynamicResourceData* resource) override;

	//	HighRenderBatcher m_batcher;
	//};

	//class HighRenderCommandPostProcessing : public AbstractHighRenderCommand {
	//private:
	//	IShaderResourceData* m_vertexShader;
	//	IShaderResourceData* m_pixelShaderGammaCorrection;

	//public:
	//	HighRenderCommandPostProcessing(IRenderResourceFactory* factory);
	//	virtual ~HighRenderCommandPostProcessing();

	//	void Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) override;
	//};

	class HighRenderCommandBakeHDRIToEnvironmentCubemap : public AbstractHighRenderCommand {
	private:
		Array<Matrix4x4> m_mat4x4ViewProjection;

	public:
		HighRenderCommandBakeHDRIToEnvironmentCubemap(const HighRenderStorage& storage);
		virtual ~HighRenderCommandBakeHDRIToEnvironmentCubemap() = default;

		void Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) override;

	private:
		void BakeEnvironmentCubemap(HighRenderPipelineAdapter* pipeline, const MeshUnit& mesh, const Material* material);
		void BakeIrradianceCubemap(HighRenderPipelineAdapter* pipeline, const MeshUnit& mesh, const Material* material);

		void UpdateUBObject(BufferResource* resource) override;

		HighRenderBatcher m_batcher;
		Int32 m_faceId;
	};
}

#endif // !HIGH_RENDER_COMMAND_H
