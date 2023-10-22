#ifndef BASE_RENDER_PASS_H
#define BASE_RENDER_PASS_H

#include "Engine/Rendering/Engine/Interface/IRenderPass.h"

namespace Engine {
	class Mesh;

	struct BRPModelResource {
		Mesh* meshes;
		Matrix4x4 world;
	};

	struct BRPCameraResource {
		Matrix4x4 view;
		Matrix4x4 proj;
	};

	class BaseRenderPass : public IRenderPass {
	public:
		BaseRenderPass(IContext* context, IShaderResourceData* vertexShader);
		virtual ~BaseRenderPass();

		void Launch(IRenderPipeline* pipeline) override;
		bool Is(RenderPassType type) const override;

	private:
		Array<ITextureResourceData*> m_gbuffer;
		Array<IBufferResourceData*> m_ubuffer;
		IShaderResourceData* m_vertexShader;

		Array<BRPModelResource> m_modelResourceBatch;
		BRPCameraResource m_cameraResource;
	};
}

#endif // !BASE_RENDER_PASS_H
