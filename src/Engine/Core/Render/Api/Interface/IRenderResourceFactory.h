#ifndef IRENDER_RESOURCE_FACTORY_H
#define IRENDER_RESOURCE_FACTORY_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"

namespace Engine {
	class IRenderResourceFactory {
	public:
		virtual ITextureResourceData* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) = 0;
		virtual ITextureResourceData* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height) = 0;
	};
}

#endif // !IRENDER_RESOURCE_FACTORY_H
