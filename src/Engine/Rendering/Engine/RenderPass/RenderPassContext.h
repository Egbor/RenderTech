#ifndef RENDER_PASS_CONTEXT_H
#define RENDER_PASS_CONTEXT_H

#include "Engine/Core/Core.h"

#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"
#include "Engine/Object/Component/SceneComponent.h"

namespace Engine {
	class RenderPassContext {
	public:
		RenderPassContext(IRenderPipeline* pipeline, Int32 width, Int32 height) {
			pipeline->SetViewport(width, height);
		}

		virtual ~RenderPassContext() {
			DELETE_ARRAY_OF_OBJECTS(m_renderPassBatch);
		}

		void Append(AbstractRenderPass* pass, ISwapChain* swapchain) {
			m_renderPassBatch.push_back(pass);
			pass->Initialize(swapchain->GetOutputTarget());
		}

		void Process(Array<SceneComponent*>& components) {
			for (AbstractRenderPass* pass : m_renderPassBatch) {
				for (SceneComponent* component : components) {
					component->CreateRenderState(pass);
				}
			}
		}

		void Render(IContext* context) {
			IRenderPipeline* pipeline = context->QueryPipeline();
			ISwapChain* swapchain = context->QuerySwapChain();

			AbstractRenderPass* prevPass = nullptr;
			for (AbstractRenderPass* pass : m_renderPassBatch) {
				pass->Launch(pipeline, prevPass);
				prevPass = pass;
			}
			swapchain->Swap();
		}

	private:
		Array<AbstractRenderPass*> m_renderPassBatch;
	};
}

#endif // !RENDER_PASS_CONTEXT_H
