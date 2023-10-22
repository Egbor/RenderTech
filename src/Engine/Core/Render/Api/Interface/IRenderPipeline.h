#ifndef IRENDER_PIPELINE_H
#define IRENDER_PIPELINE_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"
#include "Engine/Core/Render/Api/Interface/IBufferResource.h"
#include "Engine/Core/Render/Api/Interface/IShaderResource.h"

namespace Engine {
	class IRenderPipeline {
	public:
		virtual void Draw(IBufferResourceData* vertex, IBufferResourceData* index) = 0;
		virtual void Update(IBufferResourceData* buffer, Int32 size, const void* data) = 0;

		virtual void SetGBuffer(const Array<ITextureResourceData*>& framebuffers) = 0;
		virtual void SetTextures(const Array<ITextureResourceData*>& resources) = 0;
		virtual void SetBuffers(const Array<IBufferResourceData*>& resources) = 0;
		virtual void SetShader(IShaderResourceData* shader) = 0;
	};
}

#endif // !IRENDER_PIPELINE_H
