#ifndef VIRTUAL_RENDER_PIPELINE_H
#define VIRTUAL_RENDER_PIPELINE_H

#include "Engine/Core/Render/Api/Interface/IRenderPipeline.h"
#include "Engine/Rendering/Engine/RenderBatcher.h"

namespace Engine {
	class VirtualRenderPipeline {
	private:
		IRenderPipeline* m_apiPipeline;
		ISwapChain* m_apiSwapChain;

		Viewport m_viewport;

		GBuffer m_gbuffer;
		UBuffer m_ubuffer;
		States m_states;

		Map<String, Int32> m_resourceIds;

	public:
		VirtualRenderPipeline(ISwapChain* apiSwapChain, IRenderPipeline* apiPipeline);
		virtual ~VirtualRenderPipeline() = default;

		void DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index);
		void DrawIndexedWaveframe(IBufferResourceData* vertex, IBufferResourceData* index);
		void SwapBuffers();

		void SetViewport(Int32 width, Int32 height);

		void ClearGBuffer(BatchSlot batchId, bool enableDepthClear, bool enableStencilClear, UInt32 stencilClearValue = 0);

		void InitResourceForGBuffer(EnumFlags<BatchSlot> batchIds, const String& tag);
		void InitResourceForGBuffer(EnumFlags<BatchSlot> batchIds, const String& tag, ITargetResourceData* resource);
		void InitResourceForGBuffer(EnumFlags<BatchSlot> batchIdx, const String& tag, TextureType type, TextureFormat format, Int32 width, Int32 height);
		void InitResourceForUBuffer(EnumFlags<BatchSlot> batchIds, const String& tag, Int32 bufferSize);
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

		ITextureResourceData* GetTargetDataFromGBuffer(const String& tag) const;
		ITargetResourceData* GetTargetFromGBuffer(const String& tag) const;

	private:
		inline void AdjustViewport();
	};
}

#endif // !VIRTUAL_RENDER_PIPELINE_H
