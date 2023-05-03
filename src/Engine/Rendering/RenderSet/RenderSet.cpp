#include "Engine/Rendering/RenderSet/RenderSet.h"

namespace Engine {
	RenderSet::RenderSet() 
		: m_swapchain(nullptr) {

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