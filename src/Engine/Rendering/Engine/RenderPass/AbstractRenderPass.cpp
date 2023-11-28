#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"

namespace Engine {
	Float AbstractRenderPass::GetRenderWidth() const {
		Viewport viewport;
		Core::GetInstance()->GetContext()->QueryPipeline()->GetViewport(viewport);
		return viewport.width;
	}

	Float AbstractRenderPass::GetRenderHeight() const {
		Viewport viewport;
		Core::GetInstance()->GetContext()->QueryPipeline()->GetViewport(viewport);
		return viewport.height;
	}

	GBuffer& AbstractRenderPass::GetGBuffer() {
		return m_gbuffer;
	}

	UBuffer& AbstractRenderPass::GetUBuffer() {
		return m_ubuffer;
	}
}