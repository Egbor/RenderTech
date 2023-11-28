#ifndef LIGHT_RENDER_PASS_H
#define LIGHT_RENDER_PASS_H

#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"

namespace Engine {
	class LightRenderPass : public AbstractRenderPass {
	public:
		LightRenderPass();
		virtual ~LightRenderPass() = default;

		void Initialize(ITargetResourceData* output) override;
		void Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev = nullptr) override;
		bool Is(RenderPassType type) const override;
	};
}

#endif // !LIGHT_RENDER_PASS_H
