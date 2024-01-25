#ifndef LIGHT_RENDER_PASS_H
#define LIGHT_RENDER_PASS_H

#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"

namespace Engine {
	struct LightQueue {
	public:
		LightQueue();
		virtual ~LightQueue() = default;

		void Pop();
		bool IsEmpty() const;

		RenderLight& Append(LightType type);
		RenderLight& Top();

	private:
		Array<RenderLight> m_queue;
		Int32 m_dirMark;
	};

	class LightRenderPass : public AbstractRenderPass {
	public:
		LightRenderPass();
		virtual ~LightRenderPass();

		void Initialize(ITargetResourceData* output) override;
		void Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev = nullptr) override;
		bool Is(RenderPassType type) const override;

		RenderLight& ReserveLightInQueue(LightType type);
		void SetViewProjection(Matrix4x4 view, Matrix4x4 proj);

	private:
		IShaderResourceData* m_vertexShader;
		IShaderResourceData* m_pixelShader;

		Int32 m_bufferObjectId;
		Int32 m_bufferObjectHelperId;
		Int32 m_bufferSystemId;
		Int32 m_bufferLightId;

		LightQueue m_lights;
	};
}

#endif // !LIGHT_RENDER_PASS_H
