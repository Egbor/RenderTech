#ifndef HIGH_RENDER_PIPELINE_H
#define HIGH_RENDER_PIPELINE_H

#include "Engine/Core/Render/Api/Interface/IRenderPipeline.h"
#include "Engine/Rendering/Engine/RenderBatcher.h"

namespace Engine {
	class HighRenderPipeline {
	private:
		IRenderPipeline* m_apiPipeline;
		ISwapChain* m_apiSwapChain;

		GBuffer m_gbuffer;
		UBuffer m_ubuffer;
		States m_states;

		Map<String, Int32> m_ubuffersIds;

	public:
		HighRenderPipeline(ISwapChain* apiSwapChain, IRenderPipeline* apiPipeline);
		virtual ~HighRenderPipeline() = default;

		void DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index);
		void DrawIndexedWaveframe(IBufferResourceData* vertex, IBufferResourceData* index);
		void SwapBuffers();

		void ClearGBuffer(BatchSlot batchId, bool enableDepthClear, bool enableStencilClear, UInt32 stencilClearValue = 0);

		void InitResourceForGBuffer(EnumFlags<BatchSlot> batchIds, ITargetResourceData* resource);
		void InitResourceForGBuffer(EnumFlags<BatchSlot> batchIdx, TextureType type, TextureFormat format, Int32 width, Int32 height);
		void InitResourceForUBuffer(EnumFlags<BatchSlot> batchIds, const String& bufferTag, Int32 bufferSize);
		void InitResourceForStates(EnumFlags<BatchSlot> batchIds, StateType type, StateData data);

		void BindGBuffer(BatchSlot batchId);
		void BindGBuffer(BatchSlot batchId, RenderStage stage);
		void BindUBuffer(BatchSlot batchId, RenderStage stage);
		void BindStates(BatchSlot batchId, RenderStage stage);
		void BindStates(BatchSlot batchId);

		void BindShader(RenderStage stage, IShaderResourceData* resource);
		void BindTexture(RenderStage stage, const Array<ITextureResourceData*>& resources);

		void UpdateUBuffer(const String& bufferTag, std::function<void(RawData&)> updateCallback);

		Int32 GetRenderSpaceWidth() const;
		Int32 GetRenderSpaceHeight() const;
	};
}

#endif // !HIGH_RENDER_PIPELINE_H
