#include "Engine/Rendering/RenderSet/RenderSet.h"

namespace Engine {
	RenderSet::RenderSet(Texture2D* target) 
		: m_swapchain(nullptr), m_target(target) {

	}

	RenderSet::~RenderSet() {
		DELETE_ARRAY_OF_OBJECTS(m_renderPass);
	}

	void RenderSet::Execute() {
		PreExecute();
		RenderPass* prev = nullptr;
		for (auto it = m_renderPass.begin(); it != m_renderPass.end(); it++) {
			(*it)->Render(prev);
			prev = *it;
		}
		PostExecute();
	}

	Texture2D* RenderSet::GetTarget() const {
		return m_target;
	}

	void RenderSet::SetSwapChain(SwapChain* swapchain) {
		m_swapchain = swapchain;
	}

	void RenderSet::PreExecute() {

	}

	void RenderSet::PostExecute() {
		if (m_swapchain != nullptr) {
			m_swapchain->SwapBuffers();
		}
	}
}