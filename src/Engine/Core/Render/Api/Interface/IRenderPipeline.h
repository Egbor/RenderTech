#ifndef IRENDER_PIPELINE_H
#define IRENDER_PIPELINE_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"

namespace Engine {
	class IRenderPipeline {
	public:
		virtual void SetGBuffer(const Array<ITextureResourceData*>& framebuffers) = 0;
		virtual void SetTextures(const Array<ITextureResourceData*>& resources) = 0;
	};
}

#endif // !IRENDER_PIPELINE_H
