#ifndef RENDER_PASS_CONTEXT_H
#define RENDER_PASS_CONTEXT_H

#include "Engine/Rendering/Engine/Interface/IRenderPassContext.h"
#include "Engine/Core/Core.h"

namespace Engine {
	class RenderPassContext : public IRenderPassContext {
	public:
		RenderPassContext(IWindow* window) {
			ChangeRenderResolution(window->GetWidth(), window->GetHeight());
		}

		virtual ~RenderPassContext() {
			DELETE_ARRAY_OF_OBJECTS(m_renderPassBatch);
		}

		void Append(IRenderPass* pass) override {
			IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
			ISwapChain* swapchain = Core::GetInstance()->GetContext()->QuerySwapChain();

			m_renderPassBatch.push_back(pass);
			pass->Create(factory, swapchain->GetWidth(), swapchain->GetHeight());
		}

		void ChangeRenderResolution(Int32 width, Int32 height) {
			IRenderPipeline* pipeline = Core::GetInstance()->GetContext()->QueryPipeline();
			pipeline->SetViewport(width, height);
		}

		void Process(Array<SceneComponent*>& components) override {
			for (IRenderPass* pass : m_renderPassBatch) {
				for (SceneComponent* component : components) {
					component->CreateRenderState(pass);
				}
			}
		}

		void Render() override {
			IRenderPipeline* pipeline = Core::GetInstance()->GetContext()->QueryPipeline();
			ISwapChain* swapchain = Core::GetInstance()->GetContext()->QuerySwapChain();

			for (IRenderPass* pass : m_renderPassBatch) {
				pass->Launch(pipeline);
			}
			swapchain->Swap();
		}

	private:
		Array<IRenderPass*> m_renderPassBatch;
	};
}

#endif // !RENDER_PASS_CONTEXT_H
