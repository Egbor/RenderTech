#ifndef BASE_RENDER_PASS_H
#define BASE_RENDER_PASS_H

#include "Engine/Rendering/Engine/Interface/IRenderPass.h"

namespace Engine {
	class BaseRenderPass : public IRenderPass {
	public:
		BaseRenderPass(IContext* context);
		virtual ~BaseRenderPass();

		void Launch(IRenderPipeline* pipeline) override;
		bool Is(RenderPassType type) const override;

	private:
		Array<ITextureResourceData*> m_gbuffer;
	};
}

#endif // !BASE_RENDER_PASS_H
