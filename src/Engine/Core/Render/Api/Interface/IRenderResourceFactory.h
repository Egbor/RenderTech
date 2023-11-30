#ifndef IRENDER_RESOURCE_FACTORY_H
#define IRENDER_RESOURCE_FACTORY_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"
#include "Engine/Core/Render/Api/Interface/ITargetResource.h"
#include "Engine/Core/Render/Api/Interface/IBufferResource.h"
#include "Engine/Core/Render/Api/Interface/IShaderResource.h"

namespace Engine {
	class IRenderResourceFactory {
	public:
		virtual ~IRenderResourceFactory() = default;
		virtual ITextureResourceData* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) = 0;
		virtual ITargetResourceData* CreateTarget(TextureType type, TextureFormat format, Int32 width, Int32 height) = 0;
		virtual IBufferResourceData* CreateBuffer(BufferType type, Int32 size, Int32 strides, const void* data) = 0;
		virtual IShaderResourceData* CreateShader(ShaderType type, Size codeLength, const void* code) = 0;
	};
}

#endif // !IRENDER_RESOURCE_FACTORY_H
