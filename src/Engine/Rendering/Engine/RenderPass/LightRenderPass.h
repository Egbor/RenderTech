#ifndef LIGHT_RENDER_PASS_H
#define LIGHT_RENDER_PASS_H

#include "Engine/Rendering/Engine/Interface/IRenderPass.h"

namespace Engine {
	class LightRenderPass : public IRenderPass {
	public:
		LightRenderPass();
		virtual ~LightRenderPass() = default;

		void Create(IRenderResourceFactory* factory, Int32 width, Int32 height) override;
		void Launch(IRenderPipeline* pipeline) override;
		bool Is(RenderPassType type) const override;
	};
}

#endif // !LIGHT_RENDER_PASS_H
