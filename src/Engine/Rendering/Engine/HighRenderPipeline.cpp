#include "Engine/Rendering/Engine/HighRenderPipeline.h"

namespace Engine {
	HighRenderPipeline::HighRenderPipeline(ISwapChain* apiSwapChain, IRenderPipeline* apiPipeline)
		: m_apiSwapChain(apiSwapChain), m_apiPipeline(apiPipeline), m_gbuffer(), m_ubuffer(), m_states() {
		m_apiPipeline->SetViewport(apiSwapChain->GetWidth(), apiSwapChain->GetHeight());
	}

	void HighRenderPipeline::DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index) {
		m_apiPipeline->Draw(vertex, index);
	}

	void HighRenderPipeline::DrawIndexedWaveframe(IBufferResourceData* vertex, IBufferResourceData* index) {
		m_apiPipeline->DrawWaveframe(vertex, index);
	}

	void HighRenderPipeline::SwapBuffers() {
		m_apiSwapChain->Swap();
	}

	void HighRenderPipeline::ClearGBuffer(BatchSlot batchId, bool enableDepthClear, bool enableStencilClear, UInt32 stencilClearValue) {
		m_gbuffer.Clear(batchId, enableDepthClear, enableStencilClear, stencilClearValue);
	}

	void HighRenderPipeline::InitResourceForGBuffer(EnumFlags<BatchSlot> batchIds, ITargetResourceData* resource) {
		m_gbuffer.InitNewResource(batchIds, resource);
	}

	void HighRenderPipeline::InitResourceForGBuffer(EnumFlags<BatchSlot> batchIdx, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		m_gbuffer.InitNewResource(batchIdx, type, format, width, height);
	}

	void HighRenderPipeline::InitResourceForUBuffer(EnumFlags<BatchSlot> batchIds, const String& bufferTag, Int32 bufferSize) {
		m_ubuffersIds[bufferTag] = 0;
		m_ubuffer.InitNewResource(batchIds, bufferSize, &m_ubuffersIds[bufferTag]);
	}

	void HighRenderPipeline::InitResourceForStates(EnumFlags<BatchSlot> batchIds, StateType type, StateData data) {
		m_states.InitNewResource(batchIds, type, data);
	}

	void HighRenderPipeline::BindGBuffer(BatchSlot batchId) {
		m_gbuffer.Bind(batchId, m_apiPipeline);
	}

	void HighRenderPipeline::BindGBuffer(BatchSlot batchId, RenderStage stage) {
		m_gbuffer.Bind(batchId, m_apiPipeline->GetStage(stage));
	}

	void HighRenderPipeline::BindUBuffer(BatchSlot batchId, RenderStage stage) {
		m_ubuffer.Bind(batchId, m_apiPipeline->GetStage(stage));
	}

	void HighRenderPipeline::BindStates(BatchSlot batchId, RenderStage stage) {
		m_states.Bind(batchId, m_apiPipeline->GetStage(stage));
	}

	void HighRenderPipeline::BindStates(BatchSlot batchId) {
		m_states.Bind(batchId, m_apiPipeline);
	}

	void HighRenderPipeline::BindShader(RenderStage stage, IShaderResourceData* resource) {
		IRenderStage* renderStage = m_apiPipeline->GetStage(stage);
		renderStage->BindShader(resource);
	}

	void HighRenderPipeline::BindTexture(RenderStage stage, const Array<ITextureResourceData*>& resources) {
		IRenderStage* renderStage = m_apiPipeline->GetStage(stage);
		renderStage->BindTextures(resources);
	}

	void HighRenderPipeline::UpdateUBuffer(const String& bufferTag, std::function<void(RawData&)> updateCallback) {
		Int32 bufferId = m_ubuffersIds[bufferTag];
		updateCallback(m_ubuffer.GetBufferData(bufferId));
		m_ubuffer.Update(bufferId);
	}

	Int32 HighRenderPipeline::GetRenderSpaceWidth() const {
		Viewport viewport;
		m_apiPipeline->GetViewport(viewport);
		return static_cast<Int32>(viewport.width);
	}

	Int32 HighRenderPipeline::GetRenderSpaceHeight() const {
		Viewport viewport;
		m_apiPipeline->GetViewport(viewport);
		return static_cast<Int32>(viewport.height);
	}
}
