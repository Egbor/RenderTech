#ifndef IRENDER_RESOURCE_FACTORY_H
#define IRENDER_RESOURCE_FACTORY_H

#include "Engine/Core/Render/Base/Resource/TextureResource.h"
#include "Engine/Core/Render/Base/Resource/TargetResource.h"
#include "Engine/Core/Render/Base/Resource/BufferResource.h"
#include "Engine/Core/Render/Base/Resource/ShaderResource.h"
#include "Engine/Core/Render/Base/Resource/StateResource.h"

namespace Engine {
	class IRenderResourceFactory {
	public:
		virtual ~IRenderResourceFactory() = default;
		virtual StateResource* CreateState(StateType type, StateData data) = 0;
		virtual TargetResource* CreateTarget(TextureType type, TextureFormat format, Int32 width, Int32 height) = 0;
		virtual BufferResource* CreateBuffer(BufferType type, Int32 size, Int32 strides, const void* data) = 0;
		virtual ShaderResource* CreateShader(RenderStage stage, Size codeLength, const void* code) = 0;
		virtual TextureResource* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) = 0;
	};
}

#endif // !IRENDER_RESOURCE_FACTORY_H
