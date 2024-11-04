#include "Engine/Rendering/Engine/HighRenderPipeline.h"

namespace Engine {
	HighRenderPipelineAdapter::HighRenderPipelineAdapter(IRenderPipeline* pipeline)
		: m_pipeline(pipeline) {

	}

	void HighRenderPipelineAdapter::SetViewportResolution(Int32 width, Int32 height) {
		m_pipeline->SetViewport(width, height);
	}

	void HighRenderPipelineAdapter::DisableRenderStage(RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->Disable();
	}

	void HighRenderPipelineAdapter::EnableRenderStage(RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->Enable();
	}

	template<>
	constexpr void HighRenderPipelineAdapter::BindResources<IStateResourceData>(const Array<IStateResourceData*>& resources) {
		m_pipeline->SetStates(resources);
	}

	template<>
	constexpr void HighRenderPipelineAdapter::BindResources<ITargetResourceData>(const Array<ITargetResourceData*>& resources) {
		m_pipeline->SetTargets(resources);
	}

	template<>
	constexpr void HighRenderPipelineAdapter::BindResources<IShaderResourceData>(const Array<IShaderResourceData*>& resources) {
		for (Int32 i = 0; i < static_cast<Int32>(RenderStage::RS_NUM); i++) {
			IRenderStage* _stage = m_pipeline->GetStage(static_cast<RenderStage>(i));
			_stage->BindShader(resources[i]);
		}
	}

	template<>
	constexpr void HighRenderPipelineAdapter::BindResources<IStateResourceData>(const Array<IStateResourceData*>& resources, RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->BindSamplers(resources);
	}

	template<>
	constexpr void HighRenderPipelineAdapter::BindResources<ITextureResourceData>(const Array<ITextureResourceData*>& resources, RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->BindTextures(resources);
	}

	template<>
	constexpr void HighRenderPipelineAdapter::BindResources<IBufferResourceData>(const Array<IBufferResourceData*>& resources, RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->BindBuffers(resources);
	}

	IRenderPipeline* HighRenderPipelineAdapter::GetDirectAccessToPipeline() const {
		return m_pipeline;
	}
}