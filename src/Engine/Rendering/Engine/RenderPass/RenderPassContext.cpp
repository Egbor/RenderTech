#include "Engine/Rendering/Engine/RenderPass/RenderPassContext.h"

namespace Engine {
	void RenderPassContext::Append(IRenderPass* pass) {
		m_renderPassBatch.push_back(pass);
	}

	void RenderPassContext::Process(EventBase<IRenderPass*>& callback) {
		for (IRenderPass* pass : m_renderPassBatch) {
			callback.Invoke(pass);
		}
	}
}