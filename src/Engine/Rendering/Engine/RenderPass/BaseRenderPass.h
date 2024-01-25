#ifndef BASE_RENDER_PASS_H
#define BASE_RENDER_PASS_H

#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"

namespace Engine {
	class BaseRenderPass : public AbstractRenderPass {
	public:
		BaseRenderPass();
		virtual ~BaseRenderPass();

		void Initialize(ITargetResourceData* output) override;
		void Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev = nullptr) override;
		bool Is(RenderPassType type) const override;

		RenderModel& ReserveModelInQueue();
		void SetCamera(Matrix4x4 view, Matrix4x4 proj);

	private:
		void UpdateBaseBuffer(RenderModel& model);

		Int32 m_bufferObjectId;
		Int32 m_bufferObjectHelperId;

		IShaderResourceData* m_vertexShader;
		Array<RenderModel> m_queue;
	};
}

#endif // !BASE_RENDER_PASS_H
