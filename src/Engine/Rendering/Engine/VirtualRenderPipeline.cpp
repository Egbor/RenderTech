#include "Engine/Rendering/Engine/VirtualRenderPipeline.h"

namespace Engine {
	VirtualRenderPipeline::VirtualRenderPipeline(ISwapChain* apiSwapChain, IRenderPipeline* apiPipeline)
		: m_viewport(apiSwapChain->GetWidth(), apiSwapChain->GetHeight()), m_apiSwapChain(apiSwapChain), m_apiPipeline(apiPipeline)
		, m_gbuffer(), m_ubuffer(), m_states() {
	}

	void VirtualRenderPipeline::DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index) {
		AdjustViewport();
		m_apiPipeline->Draw(vertex, index);
	}

	void VirtualRenderPipeline::DrawIndexedWaveframe(IBufferResourceData* vertex, IBufferResourceData* index) {
		AdjustViewport();
		m_apiPipeline->DrawWaveframe(vertex, index);
	}

	void VirtualRenderPipeline::SwapBuffers() {
		m_apiSwapChain->Swap();
	}

	void VirtualRenderPipeline::ClearGBuffer(BatchSlot batchId, bool enableDepthClear, bool enableStencilClear, UInt32 stencilClearValue) {
		m_gbuffer.Clear(batchId, enableDepthClear, enableStencilClear, stencilClearValue);
	}

	void VirtualRenderPipeline::InitResourceForGBuffer(EnumFlags<BatchSlot> batchIds) {
		m_gbuffer.InitNewResource(batchIds, m_apiSwapChain->GetOutputTarget());
	}

	void VirtualRenderPipeline::InitResourceForGBuffer(EnumFlags<BatchSlot> batchIds, ITargetResourceData* resource) {
		m_gbuffer.InitNewResource(batchIds, resource);
	}

	void VirtualRenderPipeline::InitResourceForGBuffer(EnumFlags<BatchSlot> batchIdx, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		m_gbuffer.InitNewResource(batchIdx, type, format, width, height);
	}

	void VirtualRenderPipeline::InitResourceForUBuffer(EnumFlags<BatchSlot> batchIds, const String& bufferTag, Int32 bufferSize) {
		m_ubuffersIds[bufferTag] = 0;
		m_ubuffer.InitNewResource(batchIds, bufferSize, &m_ubuffersIds[bufferTag]);
	}

	void VirtualRenderPipeline::InitResourceForStates(EnumFlags<BatchSlot> batchIds, StateType type, StateData data) {
		m_states.InitNewResource(batchIds, type, data);
	}

	void VirtualRenderPipeline::BindGBuffer(BatchSlot batchId) {
		m_gbuffer.Bind(batchId, m_apiPipeline);
	}

	void VirtualRenderPipeline::BindGBuffer(BatchSlot batchId, RenderStage stage) {
		m_gbuffer.Bind(batchId, m_apiPipeline->GetStage(stage));
	}

	void VirtualRenderPipeline::BindUBuffer(BatchSlot batchId, RenderStage stage) {
		m_ubuffer.Bind(batchId, m_apiPipeline->GetStage(stage));
	}

	void VirtualRenderPipeline::BindStates(BatchSlot batchId, RenderStage stage) {
		m_states.Bind(batchId, m_apiPipeline->GetStage(stage));
	}

	void VirtualRenderPipeline::BindStates(BatchSlot batchId) {
		m_states.Bind(batchId, m_apiPipeline);
	}

	void VirtualRenderPipeline::BindShader(RenderStage stage, IShaderResourceData* resource) {
		IRenderStage* renderStage = m_apiPipeline->GetStage(stage);
		renderStage->BindShader(resource);
	}

	void VirtualRenderPipeline::BindTexture(RenderStage stage, const Array<ITextureResourceData*>& resources) {
		IRenderStage* renderStage = m_apiPipeline->GetStage(stage);
		renderStage->BindTextures(resources);
	}

	void VirtualRenderPipeline::UpdateUBuffer(const String& bufferTag, std::function<void(RawData&)> updateCallback) {
		Int32 bufferId = m_ubuffersIds[bufferTag];
		updateCallback(m_ubuffer.GetBufferData(bufferId));
		m_ubuffer.Update(bufferId);
	}

	Int32 VirtualRenderPipeline::GetRenderSpaceWidth() const {
		return m_viewport.GetWidth();
	}

	Int32 VirtualRenderPipeline::GetRenderSpaceHeight() const {
		return m_viewport.GetHeight();
	}

	inline void VirtualRenderPipeline::AdjustViewport() {
		Viewport viewport;
		m_apiPipeline->GetViewport(viewport);

		if (viewport != m_viewport) {
			m_apiPipeline->SetViewport(m_viewport.GetWidth(), m_viewport.GetHeight());
		}
	}
}
