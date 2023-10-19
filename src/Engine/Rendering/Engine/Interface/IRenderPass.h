#ifndef IRENDER_PASS_H
#define IRENDER_PASS_H

#include "Engine/Core/Render/Api/Interface/IContext.h"

namespace Engine {
	enum class RenderPassType {
		RP_BASE,
		RP_LIGHT
	};

	class IRenderPass {
	public:
		virtual void Launch(IRenderPipeline* pipeline) = 0;
		virtual bool Is(RenderPassType type) const = 0;
	};
}

#endif // !IRENDER_PASS_H
