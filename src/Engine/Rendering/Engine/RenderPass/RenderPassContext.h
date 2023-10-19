#ifndef RENDER_PASS_CONTEXT_H
#define RENDER_PASS_CONTEXT_H

#include "Engine/Core/Utils/Event.h"
#include "Engine/Rendering/Engine/Interface/IRenderPass.h"

namespace Engine {
	class RenderPassContext {
	public:
		RenderPassContext() = default;
		virtual ~RenderPassContext() = default;

		void Append(IRenderPass* pass);
		void Process(EventBase<IRenderPass*>& callback);

	private:
		Array<IRenderPass*> m_renderPassBatch;
	};
}

#endif // !RENDER_PASS_CONTEXT_H
