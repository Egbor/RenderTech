#ifndef IRENDER_SATGE_H
#define IRENDER_STAGE_H

#include "Engine/Core/Render/Api/Interface/IRenderPipeline.h"

namespace Engine {
	class IRenderStage {
	public:
		virtual void BindTextures(const Array<ITextureResourceData*>& resources) = 0;
		virtual void BindBuffers(const Array<IBufferResourceData*>& resources) = 0;
	};
}

#endif // !IRENDER_SATGE_H
