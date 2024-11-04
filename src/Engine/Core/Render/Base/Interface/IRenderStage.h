#ifndef IRENDER_STAGE_H
#define IRENDER_STAGE_H

#include "Engine/Core/Render/Base/Resource/TextureResource.h"
#include "Engine/Core/Render/Base/Resource/BufferResource.h"
#include "Engine/Core/Render/Base/Resource/ShaderResource.h"
#include "Engine/Core/Render/Base/Resource/StateResource.h"

namespace Engine {
	class IRenderStage {
	public:
		virtual ~IRenderStage() = default;
		virtual void BindTextures(const Array<TextureResource*>& resources) = 0;
		virtual void BindBuffers(const Array<BufferResource*>& resources) = 0;
		virtual void BindSamplers(const Array<StateResource*>& resources) = 0;
		virtual void BindShader(ShaderResource* resource) = 0;

		virtual void Disable() = 0;
		virtual void Enable() = 0;
	};
}

#endif // !IRENDER_SATGE_H
