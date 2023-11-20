#ifndef RENDER_PASS_CONTEXT_H
#define RENDER_PASS_CONTEXT_H

#include "Engine/Rendering/Engine/Interface/IRenderPassContext.h"
#include "Engine/Core/System/Platform/Interface/IWindow.h"

namespace Engine {
	template<class RenderContext>
	class RenderPassContext : public IRenderPassContext {
	public:
		RenderPassContext(IWindow* window) {
			m_renderContext = new RenderContext(window);
		}

		virtual ~RenderPassContext() {
			DELETE_ARRAY_OF_OBJECTS(m_renderPassBatch);
		}

		void Append(IRenderPass* pass) override {
			m_renderPassBatch.push_back(pass);
		}

		void Process(Array<SceneComponent*>& components) override {
			for (IRenderPass* pass : m_renderPassBatch) {
				for (SceneComponent* component : components) {
					component->CreateRenderState(pass);
				}
			}
		}

		void Render() override {

		}

	private:
		IContext* m_renderContext;
		Array<IRenderPass*> m_renderPassBatch;
	};
}

#endif // !RENDER_PASS_CONTEXT_H
