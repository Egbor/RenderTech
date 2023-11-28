#ifndef IRENDER_STAGE_H
#define IRENDER_STAGE_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"
#include "Engine/Core/Render/Api/Interface/IBufferResource.h"
#include "Engine/Core/Render/Api/Interface/IShaderResource.h"

namespace Engine {
	class IRenderStage {
	public:
		virtual void BindTextures(const Array<ITextureResourceData*>& resources) = 0;
		virtual void BindBuffers(const Array<IBufferResourceData*>& resources) = 0;
		virtual void BindShader(IShaderResourceData* resource) = 0;
	};
}

#endif // !IRENDER_SATGE_H
