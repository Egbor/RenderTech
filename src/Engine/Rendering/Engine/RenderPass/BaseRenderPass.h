#ifndef BASE_RENDER_PASS_H
#define BASE_RENDER_PASS_H

#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
	struct RenderModel {
		Matrix4x4 world;
		Mesh* mesh;
	};

	class BaseRenderPass : public AbstractRenderPass {
	public:
		BaseRenderPass();
		virtual ~BaseRenderPass();

		void Initialize(ITargetResourceData* output) override;
		void Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev = nullptr) override;
		bool Is(RenderPassType type) const override;

		void SetModel(Matrix4x4 world, Mesh* mesh);
		void SetViewProjection(Matrix4x4 viewproj);

	private:
		Mesh* UpdateAndGetFrontMesh();

		IShaderResourceData* m_vertexShader;
		Queue<RenderModel> m_models;
	};
}

#endif // !BASE_RENDER_PASS_H
