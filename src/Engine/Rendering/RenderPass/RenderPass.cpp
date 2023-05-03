#include "Engine/Rendering/RenderPass/RenderPass.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	RenderPass::RenderPass(Context* context) 
		: m_targets(new TargetMap(context)) {

	}

	RenderPass::~RenderPass() {
		DELETE_OBJECT(m_targets);
	}

	Target* RenderPass::GetTarget(RenderOutput slot) const {
		return m_targets->GetTarget(slot);
	}

	void RenderPass::ClearTarget(RenderOutput slot, TargetClear clear) {
		m_targets->GetTarget(slot)->Clear(clear);
	}

	void RenderPass::AddTarget(RenderOutput slot, Target* target) {
		m_targets->AddTarget(slot, target);
	}

	void RenderPass::AddTarget(RenderOutput slot, Texture2D* texture) {
		m_targets->AddTarget(slot, texture);
	}

	void RenderPass::AddTarget(RenderOutput slot, TextureFormat format, UInt32 width, UInt32 height) {
		m_targets->AddTarget(slot, format, width, height);
	}

	void RenderPass::RemoveTarget(RenderOutput slot) {
		m_targets->RemoveTarget(slot);
	}

	void RenderPass::Render(RenderPass* prev) {

	}
}
