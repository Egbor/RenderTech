#ifndef HIGH_RENDER_PIPELINE_H
#define HIGH_RENDER_PIPELINE_H

#include "Engine/Core/Render/Api/Interface/IRenderPipeline.h"
#include "Engine/Rendering/Engine/RenderBatcher.h"

namespace Engine {
	class HighRenderPipeline {
	private:
		IRenderPipeline* m_apiPipeline;

		GBuffer m_gbuffer;
		UBuffer m_ubiffer;
		States m_states;

	public:
		HighRenderPipeline(IRenderPipeline* apiPipeline);
		virtual ~HighRenderPipeline() = default;

		void DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index);

		void ClearGBuffer(BatchSlot batchId);

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
		void BindTexture(RenderStage stage, const Array<ITextureResourceData*> resources);

		void UpdateUBuffer(const String& bufferTag, std::function<void(RawData&)> updateCallback);

		Float GetRenderSpaceWidth() const;
		Float GetRenderSpaceHeight() const;
	};
}

#endif // !HIGH_RENDER_PIPELINE_H
