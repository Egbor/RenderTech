#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

namespace Engine {
	LightRenderPass::LightRenderPass() {

	}

	void LightRenderPass::Initialize(ITargetResourceData* output) {
		GetGBuffer().Attach(output);
	}

	void LightRenderPass::Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev) {
		pipeline->SetTargets(GetGBuffer().GetTargets());

		GetGBuffer().Clear();
	}

	bool LightRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_LIGHT;
	}
}