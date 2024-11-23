#include "Engine/Rendering/Engine/HighRenderPipeline.h"

namespace Engine {
	HighRenderPipelineAdapter::HighRenderPipelineAdapter(IRenderPipeline* pipeline) noexcept
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
	void HighRenderPipelineAdapter::BindResources<StateResource>(const Array<StateResource*>& resources) {
		m_pipeline->SetStates(resources);
	}

	template<>
	void HighRenderPipelineAdapter::BindResources<TargetResource>(const Array<TargetResource*>& resources) {
		m_pipeline->SetTargets(resources);
	}

	template<>
	 void HighRenderPipelineAdapter::BindResources<ShaderResource>(const Array<ShaderResource*>& resources) {
		for (Int32 i = 0; i < static_cast<Int32>(RenderStage::RS_NUM); i++) {
			IRenderStage* _stage = m_pipeline->GetStage(static_cast<RenderStage>(i));
			_stage->BindShader(resources[i]);
		}
	}

	template<>
	void HighRenderPipelineAdapter::BindResources<StateResource>(const Array<StateResource*>& resources, RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->BindSamplers(resources);
	}

	template<>
	void HighRenderPipelineAdapter::BindResources<TextureResource>(const Array<TextureResource*>& resources, RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->BindTextures(resources);
	}

	template<>
	void HighRenderPipelineAdapter::BindResources<BufferResource>(const Array<BufferResource*>& resources, RenderStage stage) {
		IRenderStage* _stage = m_pipeline->GetStage(stage);
		_stage->BindBuffers(resources);
	}

	IRenderPipeline* HighRenderPipelineAdapter::GetDirectAccessToPipeline() const {
		return m_pipeline;
	}
}