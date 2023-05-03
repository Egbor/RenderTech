#ifndef RENDERSET
#define RENDERSET

#include "Engine/Rendering/RenderPass/RenderPass.h"
#include "Engine/Core/Render/Api/SwapChain.h"

namespace Engine {
	class RenderSet {
	private:
		Array<RenderPass*> m_renderPass;
		SwapChain* m_swapchain;

	public:
		RenderSet();
		virtual ~RenderSet();

		void Execute();
		void SetSwapChain(SwapChain* swapchain);

		template<class TClass, class ...TArgs>
		Int32 AddRenderPass(TArgs... args) {
			m_renderPass.push_back(new TClass(args...));
			return m_renderPass.size() - 1;
		}

	protected:
		virtual void PreExecute();
		virtual void PostExecute();
	};
}

#endif // !RENDERSET
