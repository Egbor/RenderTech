#ifndef BASE_RENDER_PASS_H
#define BASE_RENDER_PASS_H

#include "Engine/Rendering/Engine/Interface/IRenderPass.h"
#include "Engine/Rendering/Engine/RenderPass/GBuffer.h"
#include "Engine/Object/Component/MeshComponent.h"

namespace Engine {
	class BaseRenderPassQueue {
	public:
		BaseRenderPassQueue();
		virtual ~BaseRenderPassQueue() = default;

		void SetViewProjection(Matrix4x4 viewproj);

		Mesh* GetMesh();
		UniformBufferBase& GetUniform();

		bool IsEmpty() const;
		void Add(MeshComponent* component);
		void Next();

	private:
		struct BaseRenderModel {
			Matrix4x4 world;
			Mesh* mesh;
		};

		Queue<BaseRenderModel> m_modelBatch;
		UniformBufferBase m_uniform;
	};

	class BaseRenderPass : public IRenderPass {
	public:
		BaseRenderPass();
		virtual ~BaseRenderPass();

		void Create(IRenderResourceFactory* factory, Int32 width, Int32 height) override;
		void Launch(IRenderPipeline* pipeline) override;
		bool Is(RenderPassType type) const override;

		void AddToQueue(MeshComponent* component);
		void SetViewProjection(Matrix4x4 viewproj);

	private:
		IShaderResourceData* m_vertexShader;

		GBuffer m_gbuffer;
		UBuffer m_ubuffer;

		BaseRenderPassQueue* m_queue;
	};
}

#endif // !BASE_RENDER_PASS_H
